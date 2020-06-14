#include "dbops.h"

void evalOp(string inBuffer) {
  // Put op
  if(regex_match(inBuffer, putRegex)) {
    pair<string, string> keyValuePair = getKeyValuePair(inBuffer);
    putOp(keyValuePair.first, keyValuePair.second);
  }
  // Get op
  else if(regex_match(inBuffer, getRegex)) {
    string key = getKey(inBuffer);
    getOp(key);
  }
  // No op
  else {
    cout<<"Invalid Op"<<endl;
  }
}

//-----------------------------------------------------------------------------

void putOp(string key, string value) {
  cout<<"KEY = "<<key<<endl;
  cout<<"VALUE = "<<value<<endl;
}

//-----------------------------------------------------------------------------

void getOp(string& key) {
  cout<<"KEY = "<<key<<endl;
}

//-----------------------------------------------------------------------------
