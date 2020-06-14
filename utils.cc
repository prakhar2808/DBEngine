#include "utils.h"

pair<string, string> getKeyValuePair(string& inBuffer) {
  int i = 0;
  // Operator
  while(inBuffer[i] != '<') {
    ++i;
  }
  // Key
  string key = "";
  ++i;
  while(inBuffer[i] != '>') {
    key += inBuffer[i];
    ++i;
  }
  // Value
  string value = "";
  while(inBuffer[i] != '<') {
    ++i;
  }
  ++i;
  while(inBuffer[i] != '>') {
    value += inBuffer[i];
    ++i;
  }
  return make_pair(key, value);
}

//-----------------------------------------------------------------------------

string getKey(string& inBuffer) {
  int i = 0;
  // Operator
  while(inBuffer[i] != '<') {
    ++i;
  }
  // Key
  string key = "";
  ++i;
  while(inBuffer[i] != '>') {
    key += inBuffer[i];
    ++i;
  }
  return key;
}

//-----------------------------------------------------------------------------

int getIndex(string str, int start, char ch) {
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
