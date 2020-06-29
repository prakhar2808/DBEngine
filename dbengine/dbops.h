#ifndef DB_OPS_H
#define DB_OPS_H

#include "operation.h"
#include "db.h"
#include "keyValuePairStruct.h"
#include "utils.h"

#define exitRegex std::regex("[ ]*((EXIT)|(exit))[ ]*")
#define putRegex std::regex("([ ]*)((PUT)|(put))([ ]*)(<)([ -~]+)(>)([ ]*)(<)([ -~]+)(>)([ ]*)")
#define getRegex std::regex("([ ]*)((GET)|(get))([ ]*)(<)([ -~]+)(>)([ ]*)")
#define deleteRegex std::regex("([ ]*)((DELETE)|(delete))([ ]*)(<)([ -~]+)(>)([ ]*)")
#define listAllRegex std::regex("[ ]*((LISTALL)|(listall))[ ]*")

opStatus evalOp(std::string& inBuffer, database* dbObject, int clientSocket);
opStatus putOp(keyValuePair_t keyValuePair, 
               int clientSocket, 
               database* dbObject);
opStatus filePutOp(std::string& inBuffer, int clientSocket, database* dbObject);
opStatus getOp(std::string key, int clientSocket, database* dbObject);
opStatus deleteOp(std::string key, int clientSocket, database* dbObject);
opStatus listAllOp(int clientSocket, database* dbObject);

#endif
