#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <ostream>
#include <regex>
#include <string>
#include <utility>

#include "../dbengine/dbops.h"

// Send string to client
void sendToClient(int client_socket, std::string response);
// Send char to client
void sendToClient(int client_socket, unsigned char response);
// Send message termination to client
void sendEndMsgToClient(int client_socket);

#endif
