#include "utils.h"

keyValuePair_t getKeyValuePair(std::string inBuffer) {
  int i = 0;
  // Operator
  while(inBuffer[i] != '<') {
    ++i;
  }
  // Key
  std::string key = "";
  ++i;
  while(inBuffer[i] != '>') {
    key += inBuffer[i];
    ++i;
  }
  // Value
  std::string value = "";
  while(inBuffer[i] != '<') {
    ++i;
  }
  ++i;
  while(inBuffer[i] != '>') {
    value += inBuffer[i];
    ++i;
  }
  return keyValuePair_t(key, value);
}

//-----------------------------------------------------------------------------

std::string getKey(std::string inBuffer) {
  int i = 0;
  // Operator
  while(inBuffer[i] != '<') {
    ++i;
  }
  // Key
  std::string key = "";
  ++i;
  while(inBuffer[i] != '>') {
    key += inBuffer[i];
    ++i;
  }
  return key;
}

//-----------------------------------------------------------------------------

int getIndex(std::string str, int start, char ch) {
  int len = str.length();
  if(start >= len) {
    return -1;
  }
  for(int i = start; i < len; i++) {
    if(str[i] == ch) {
      return i;
    }
  }
  return -1;
}

//-----------------------------------------------------------------------------
