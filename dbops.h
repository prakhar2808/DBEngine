#ifndef DB_OPS_H
#define DB_OPS_H

#include "utils.h"

using namespace std;

#define exitRegex regex("[ ]*((EXIT)|(exit))[ ]*")
#define putRegex regex("([ ]*)((PUT)|(put))([ ]*)(<)([ -~]+)(>)([ ]*)(<)([ -~]*)(>)([ ]*)")
#define getRegex regex("([ ]*)((GET)|(get))([ ]*)(<)([ -~]+)(>)([ ]*)")

void evalOp(string inBuffer);
void putOp(string key, string value);
void getOp(string& key);

#endif
