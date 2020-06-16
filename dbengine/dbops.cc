#include "dbops.h"

std::string evalOp(std::string inBuffer, database* dbObject) {
  // Return value
  std::string ret;
  // Put op
  if(regex_match(inBuffer, putRegex)) {
    keyValuePair_t keyValuePair = getKeyValuePair(inBuffer);
    ret = putOp(keyValuePair, dbObject);
  }
  // Get op
  else if(regex_match(inBuffer, getRegex)) {
    std::string key = getKey(inBuffer);
    ret = getOp(key, dbObject);
  }
  // No op
  else {
    ret = "ERROR: Invalid Operation!";
  }
  return ret;
}

//-----------------------------------------------------------------------------

std::string putOp(keyValuePair_t keyValuePair, database* dbObject) {
  std::cout << "Putting key = <"
       << keyValuePair.key << ">, value = <"
       << keyValuePair.value << "> ..." << std::endl;
  return dbObject->writeKeyValuePair(keyValuePair);
}

//-----------------------------------------------------------------------------

std::string getOp(std::string key, database* dbObject) {
  std::cout << "Getting value from the database for the key <"
       << key << "> ..." << std::endl;
  return dbObject->readValueFromKey(key);
}

//-----------------------------------------------------------------------------
