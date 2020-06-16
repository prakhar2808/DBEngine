#ifndef DB_OPS_H
#define DB_OPS_H

#include "db.h"
#include "keyValuePairStruct.h"
#include "utils.h"

#define exitRegex std::regex("[ ]*((EXIT)|(exit))[ ]*")
#define putRegex std::regex("([ ]*)((PUT)|(put))([ ]*)(<)([ -~]+)(>)([ ]*)(<)([ -~]*)(>)([ ]*)")
#define getRegex std::regex("([ ]*)((GET)|(get))([ ]*)(<)([ -~]+)(>)([ ]*)")

std::string evalOp(std::string inBuffer, database* dbObject);
std::string putOp(keyValuePair_t keyValuePair, database* dbObject);
std::string getOp(std::string key, database* dbObject);

#endif
