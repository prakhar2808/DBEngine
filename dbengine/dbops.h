#ifndef DB_OPS_H
#define DB_OPS_H

#include "db.h"
#include "keyValuePairStruct.h"
#include "utils.h"

#define exitRegex std::regex("[ ]*((EXIT)|(exit))[ ]*")
#define putRegex std::regex("([ ]*)((PUT)|(put))([ ]*)(<)([ -~]+)(>)([ ]*)(<)([ -~]*)(>)([ ]*)")
#define getRegex std::regex("([ ]*)((GET)|(get))([ ]*)(<)([ -~]+)(>)([ ]*)")
#define listAllRegex std::regex("[ ]*((LISTALL)|(listall))[ ]*")

bool evalOp(std::string inBuffer, database* dbObject, int clientSocket);
void putOp(keyValuePair_t keyValuePair, int clientSocket, database* dbObject);
void getOp(std::string key, int clientSocket, database* dbObject);
void listAllOp(int clientSocket, database* dbObject);

#endif
