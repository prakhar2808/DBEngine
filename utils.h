#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <ostream>
#include <regex>
#include <string>
#include <utility>

#include "dbops.h"

using namespace std;

pair<string, string> getKeyValuePair(string& inBuffer);
string getKey(string& inBuffer);
int getIndex(string str, int start, char ch);

#endif
