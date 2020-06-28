#include <iostream>
#include <string>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <sys/uio.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <fstream>
#include <chrono>

#define BUFSIZE 4096

void sendToServerAndGetResponse(std::string data, bool printResponse = true);
std::string serverIp = "127.0.0.1";
int port = 12345;
char buffer[BUFSIZE];
int clientSd;

int main(int argc, char** argv) {

  struct hostent* host = gethostbyname(serverIp.c_str()); 
  sockaddr_in sendSockAddr;   
  bzero((char*)&sendSockAddr, sizeof(sendSockAddr)); 
  sendSockAddr.sin_family = AF_INET; 
  sendSockAddr.sin_addr.s_addr = 
    inet_addr(inet_ntoa(*(struct in_addr*)*host->h_addr_list));
  sendSockAddr.sin_port = htons(port);
  clientSd = socket(AF_INET, SOCK_STREAM, 0);
    
  int status = connect(clientSd,
                      (sockaddr*) &sendSockAddr, 
                      sizeof(sendSockAddr));
  if(status < 0) {
    std::cout << "Error connecting to socket!" 
              << std::endl; 
    return status;
  }
    
  std::cout << "Connected to the server!" 
            << std::endl;

  // First message
  recv(clientSd, buffer, sizeof(buffer), 0);
  std::cout << buffer << std::endl;
  
  // Check if the client has sent a file name to dump in the db
  if(argc == 2) {
    std::cout << "Please wait! Dumping input file to the database ... "
              << std::endl;
    auto t_start = std::chrono::high_resolution_clock::now();
    std::ifstream fd;
    fd.open(argv[1]);
    fd >> std::noskipws;
    unsigned char len;
    int count = 0;
    std::string sendBuffer = "FILEPUT";
    while(fd.peek() != EOF) {
      fd >> len;
      std::string key((int)len, '\0');
      fd.read(&key[0], (int)len);
      fd >> len;
      std::string value((int)len, '\0');
      fd.read(&value[0], (int)len);
      std::string data = (char)(key.length()) 
                         + key 
                         + (char)(value.length()) 
                         + value;
      if(sendBuffer.length() + data.length() > 4000) {
        sendToServerAndGetResponse(sendBuffer, false);
        //break;
        sendBuffer = "FILEPUT";
      }
      sendBuffer += data;
      ++count;
    }
    // Do the last time.
    sendToServerAndGetResponse(sendBuffer, false);
    // Calculate time elapsed.
    auto t_end = std::chrono::high_resolution_clock::now();
    double elapsed_time_ms = 
      std::chrono::duration<double, std::milli>(t_end-t_start).count();
    std::cout << "Successfully dumped "
              << count 
              << " key-value pairs!"
              << std::endl;
    std::cout << "Time taken : "
              << (elapsed_time_ms/1000)
              << " seconds."
              << std::endl;
  }


  while(true) {
    std::cout << ">> ";
    std::string data;
    getline(std::cin, data);
    
    sendToServerAndGetResponse(data);
    
    if(data == "exit") {
      std::cout << "Exiting"
                << std::endl;
      break;
    }
  }

  // Last message
  recv(clientSd, buffer, sizeof(buffer), 0);
  std::cout << buffer << std::endl;
  close(clientSd);
  std::cout << "Connection closed" 
            << std::endl;
  return 0;    
}

void sendToServerAndGetResponse(std::string data,
                                bool printResponse) {
    memset(buffer, 0, sizeof(buffer));
    strcpy(buffer, data.c_str());
    //unsigned char len = data.length();

    //send(clientSd, &len, 1, 0);
    send(clientSd, buffer, strlen(buffer), 0);
    if(printResponse) {
      std::cout << "Awaiting server response..." 
                << std::endl;
    
      std::cout << "Response from Server: \n";
    }

    if(data == "exit") {
      return;
    }
    
    char bufferSize; //Number of bytes to be read
    while(true){
      memset(buffer, 0, sizeof(buffer));
      // Receiving length of string
      recv(clientSd, &bufferSize, sizeof(bufferSize), 0);
      // Receiving the string
      recv(clientSd, buffer, (int)bufferSize, 0);
      if(strcmp(buffer, "```end```") == 0) {
        break;
      }
      if(printResponse) {
        std::cout << buffer << std::endl;
      }
    }
}
