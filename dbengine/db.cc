#include "db.h"

database::database() {
  sstableWrapperObjRef = new sstableWrapper();
  memtableWrapperObjRef = 
    new memtableWrapper(*(sstableWrapperObjRef->sstable_no));
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
