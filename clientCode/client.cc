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

#define BUFSIZE 4096

int main(int argc, char** argv) {

  std::string serverIp = "127.0.0.1"; 
  int port = 12345;

  char buffer[BUFSIZE]; 
    
  struct hostent* host = gethostbyname(serverIp.c_str()); 
  sockaddr_in sendSockAddr;   
  bzero((char*)&sendSockAddr, sizeof(sendSockAddr)); 
  sendSockAddr.sin_family = AF_INET; 
  sendSockAddr.sin_addr.s_addr = 
    inet_addr(inet_ntoa(*(struct in_addr*)*host->h_addr_list));
  sendSockAddr.sin_port = htons(port);
  int clientSd = socket(AF_INET, SOCK_STREAM, 0);
    
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

  while(true) {
    std::cout << ">> ";
    std::string data;
    getline(std::cin, data);

    memset(buffer, 0, sizeof(buffer));
    strcpy(buffer, data.c_str());
    
    send(clientSd, buffer, strlen(buffer), 0);
    std::cout << "Awaiting server response..." 
              << std::endl;
    
    std::cout << "Response from Server: \n";
    
    if(data == "exit") {
      std::cout << "Exiting"
                << std::endl;
      break;
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
      std::cout << buffer << std::endl;
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