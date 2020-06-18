#include "db.h"

database::database()
  : memtableWrapperObjRef(new memtableWrapper()){

}

//-----------------------------------------------------------------------------

void database::writeKeyValuePair(keyValuePair_t keyValuePair,int clientSocket) {
  memtableWrapperObjRef->putKeyValuePair(keyValuePair, clientSocket);
}

//-----------------------------------------------------------------------------

void database::readValueFromKey(std::string key,int clientSocket) {
  memtableWrapperObjRef->getValueFromKey(key, clientSocket);
}

//-----------------------------------------------------------------------------

void database::readAllValues(int clientSocket) {
  memtableWrapperObjRef->getAllValues(clientSocket);
}

//----------------------------------------------------------------------------

database::~database() {
}

//-----------------------------------------------------------------------------
