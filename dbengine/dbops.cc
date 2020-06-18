#include "dbops.h"

bool evalOp(std::string inBuffer, database* dbObject, int clientSocket) {
  // Return value
  bool ret=1;
  // Put op
  if(regex_match(inBuffer, putRegex)) {
    keyValuePair_t keyValuePair = getKeyValuePair(inBuffer);
    putOp(keyValuePair, clientSocket, dbObject);
  }
  // Get op
  else if(regex_match(inBuffer, getRegex)) {
    std::string key = getKey(inBuffer);
    getOp(key, clientSocket, dbObject);
  }
  //List all
  else if(regex_match(inBuffer, listAllRegex)) {
    listAllOp(clientSocket, dbObject);
  }
  // No op
  else {
    ret = 0;
  }
  return ret;
}

//-----------------------------------------------------------------------------

void putOp(keyValuePair_t keyValuePair, int clientSocket, database* dbObject) {
  std::cout << "Putting key = <"
       << keyValuePair.key << ">, value = <"
       << keyValuePair.value << "> ..." << std::endl;
  dbObject->writeKeyValuePair(keyValuePair, clientSocket);
}

//-----------------------------------------------------------------------------

void getOp(std::string key, int clientSocket, database* dbObject) {
  std::cout << "Getting value from the database for the key <"
       << key << "> ..." << std::endl;
  dbObject->readValueFromKey(key, clientSocket);
}

//-----------------------------------------------------------------------------

void listAllOp(int clientSocket, database* dbObject) {
  std::cout << "Getting all data from the database"<<std::endl;
  dbObject->readAllValues(clientSocket);
}

//------------------------------------------------------------------------------