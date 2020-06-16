#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <ostream>
#include <regex>
#include <string>
#include <utility>

#include "keyValuePairStruct.h"

keyValuePair_t getKeyValuePair(std::string inBuffer);
std::string getKey(std::string inBuffer);
int getIndex(std::string str, int start, char ch);

#endif
