#include "dbops.h"

opStatus evalOp(std::string inBuffer, database* dbObject, int clientSocket) {
  // Put op
  if(regex_match(inBuffer, putRegex)) {
    keyValuePair_t keyValuePair = getKeyValuePair(inBuffer);
    return putOp(keyValuePair, clientSocket, dbObject);
  }
  // Get op
  else if(regex_match(inBuffer, getRegex)) {
    std::string key = getKey(inBuffer);
    return getOp(key, clientSocket, dbObject);
  }
  //List all
  else if(regex_match(inBuffer, listAllRegex)) {
    return listAllOp(clientSocket, dbObject);
  }
  // No op
  return opStatus::opInvalid;
}

//-----------------------------------------------------------------------------

opStatus putOp(keyValuePair_t keyValuePair, 
               int clientSocket, 
               database* dbObject) {
  std::cout << "Putting key = <"
       << keyValuePair.key << ">, value = <"
       << keyValuePair.value << "> ..." << std::endl;
  return dbObject->writeKeyValuePair(keyValuePair, clientSocket);
}

//-----------------------------------------------------------------------------

opStatus getOp(std::string key, int clientSocket, database* dbObject) {
  std::cout << "Getting value from the database for the key <"
       << key << "> ..." << std::endl;
  return dbObject->readValueFromKey(key, clientSocket);
}

//-----------------------------------------------------------------------------

opStatus listAllOp(int clientSocket, database* dbObject) {
  std::cout << "Getting all data from the database"
            << std::endl;
  return dbObject->readAllValues(clientSocket);
}

//------------------------------------------------------------------------------
