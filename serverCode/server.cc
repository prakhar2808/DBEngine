#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <limits.h>
#include <pthread.h>

#include <queue>
#include <string>

#include "server.h"
#include "../dbengine/db.h"
#include "../dbengine/rpel.h"

#define SERVERPORT 12345
#define BUFSIZE 4096
#define SOCKETERROR (-1)
#define SERVER_BACKLOG 100
#define THREAD_POOL_SIZE 10

pthread_t thread_pool[THREAD_POOL_SIZE];
std::queue<int*> conn_queue;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condition_var = PTHREAD_COND_INITIALIZER;
database* dbObjectRef;

typedef struct sockaddr_in SA_IN;
typedef struct sockaddr SA;

void* handle_connection(void* pclient_socket);
int check(int exp, const char* msg);
void* thread_function(void* arg);
void sendWelcomeMessage(int client_socket);
void sendGoodByeMessage(int client_socket);

int main(int argc, char** argv) {

  int server_socket, client_socket, addr_size;
  SA_IN server_addr, client_addr;

  for(int i = 0; i < THREAD_POOL_SIZE; i++) {
    pthread_create(&thread_pool[i], NULL, thread_function, NULL);
  }

  check((server_socket = socket(AF_INET, SOCK_STREAM, 0)), 
        "Failed to create socket");
  
  // Initializing address struct
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(SERVERPORT);

  check(bind(server_socket, (SA*)&server_addr, sizeof(server_addr)),
        "Bind failed!");
  check(listen(server_socket, SERVER_BACKLOG),
        "Listen failed!");

  // Create database object reference
  dbObjectRef = new database();

  while(true) {

    printf("Waiting for connections...\n");
    addr_size = sizeof(SA_IN);

    check(client_socket = 
        accept(server_socket, (SA*)&client_addr, (socklen_t*)&addr_size),
        "accept failed");
    printf("Connected!\n");
    
    // Put the connection somewhere so that an available thread can find it.
    int* pclient = (int*)malloc(sizeof(int));
    *pclient = client_socket;
    pthread_mutex_lock(&mutex);
    conn_queue.push(pclient);
    // Signal that a job has arrived in the queue.
    pthread_cond_signal(&condition_var);
    pthread_mutex_unlock(&mutex);
  }
  delete dbObjectRef;
  return 0;
}

int check(int exp, const char* msg) {
  if(exp == SOCKETERROR) {
    perror(msg);
    exit(1);
  }
  return exp;
}

void* thread_function(void* arg) {
  int* pclient;
  while(true) {
    pclient = NULL;

    pthread_mutex_lock(&mutex);
    if(conn_queue.size() == 0) {
      // The thread waits until it is signaled to wake up. The lock (mutex) is
      // also released meanwhile, and when signaled the thread first acquires
      // lock and then only returns here to continue execution.
      pthread_cond_wait(&condition_var, &mutex);
    }
    // Now there is a request in the queue to be handled.
    pclient = conn_queue.front();
    conn_queue.pop();
    pthread_mutex_unlock(&mutex);

    if(pclient != NULL) {
      handle_connection(pclient);
    }
  }
}

void* handle_connection(void* pclient_socket) {
  int client_socket = *((int*)pclient_socket);
  free(pclient_socket);
  char buffer[BUFSIZE];
  int msgsize = 0;
  int bytes_read;
  // Sending welcome message.
  sendWelcomeMessage(client_socket);
  // Getting a rpel object for the client.
  rpel* rpelObjRef = new rpel();
  // Length of the string to be read
  char responseLength;
  while(true) {
    memset(buffer, 0, sizeof(buffer));
    // Read the length of client's message.
    if((bytes_read = read(client_socket, 
                          &responseLength, 
                          sizeof(responseLength)) > 0)) {
      int msgsize = (int)responseLength;
      check(bytes_read, "recv error");

      // Reading the message.
      bytes_read = read(client_socket,
                        buffer,
                        msgsize);
      check(bytes_read, "recv error");

      printf("Request : %s\n", buffer);
      fflush(stdout);

      std::string req(buffer);

      opStatus response = rpelObjRef->execute(dbObjectRef, req, client_socket);

      if(response == opStatus::opExit) {
        break;
      }

      if(response == opStatus::opInvalid) {
        std::string responseStr = "Invalid operation!";
        sendToClient(client_socket, responseStr);
        sendEndMsgToClient(client_socket);
      }

      if(response == opStatus::opFail) {
        std::string responseStr = "Operation failed!";
        sendToClient(client_socket, responseStr);
        sendEndMsgToClient(client_socket);
      }
    }
  }

  sendGoodByeMessage(client_socket);
  close(client_socket);
  delete rpelObjRef;
  printf("Closing connection\n");
  return NULL;
}

void sendToClient(int client_socket, std::string response) {
  sendToClient(client_socket, (char)response.length());
  write(client_socket, &response[0], strlen(response.c_str()));
}


void sendToClient(int client_socket, unsigned char response) {
  write(client_socket, &response, 1);
}

void sendEndMsgToClient(int client_socket) {
  sendToClient(client_socket, "```end```"); 
}

void sendWelcomeMessage(int client_socket) {
  std::string welcome;
  
  welcome = "*************************************************************\n";
  welcome += "*                Welcome to myDB Engine                     *\n";
  welcome += "*        Created by : Prakhar Sinha, Shorya Jain            *\n";
  welcome += "*        (Type exit to terminate the connection)            *\n";
  welcome += "*************************************************************\n"; 
  
  write(client_socket, &welcome[0], strlen(welcome.c_str()));
}

void sendGoodByeMessage(int client_socket) {
  std::string goodBye;
  
  goodBye = "*************************************************************\n";
  goodBye += "*                       Good Bye!                           *\n";
  goodBye += "*************************************************************\n";

  write(client_socket, &goodBye[0], strlen(goodBye.c_str()));  
}
