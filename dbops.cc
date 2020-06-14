#include "dbops.h"

void evalOp(std::string inBuffer, database* dbObject) {
  // Put op
  if(regex_match(inBuffer, putRegex)) {
    keyValuePair_t keyValuePair = getKeyValuePair(inBuffer);
    putOp(keyValuePair, dbObject);
  }
  // Get op
  else if(regex_match(inBuffer, getRegex)) {
    std::string key = getKey(inBuffer);
    getOp(key, dbObject);
  }
  // No op
  else {
    std::cout << "Invalid Op" << std::endl;
  }
}

//-----------------------------------------------------------------------------

void putOp(keyValuePair_t keyValuePair, database* dbObject) {
  std::cout << "Putting key = <"
       << keyValuePair.key << ">, value = <"
       << keyValuePair.value << "> ..." << std::endl;
  dbObject->writeKeyValuePair(keyValuePair);
}

//-----------------------------------------------------------------------------

void getOp(std::string key, database* dbObject) {
  std::cout << "Getting value from the database for the key <"
       << key << "> ..." << std::endl;
  dbObject->readValueFromKey(key);
}

//-----------------------------------------------------------------------------
