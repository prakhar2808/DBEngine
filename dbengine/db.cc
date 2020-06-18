#include "db.h"

database::database() {
  sstableWrapperObjRef = new sstableWrapper();
  memtableWrapperObjRef = 
    new memtableWrapper(*(sstableWrapperObjRef->sstable_no));
}

//-----------------------------------------------------------------------------

opStatus database::writeKeyValuePair(keyValuePair_t keyValuePair,
                                     int clientSocket) {
  return memtableWrapperObjRef->putKeyValuePair(keyValuePair, 
                                                sstableWrapperObjRef,
                                                clientSocket);
}

//-----------------------------------------------------------------------------

opStatus database::readValueFromKey(std::string key, 
                                    int clientSocket) {
  opStatus status = memtableWrapperObjRef->getValueFromKey(key, clientSocket);
  if(status == opStatus::opFail) {
    // Check SSTables
    status = sstableWrapperObjRef->getValueFromKey(key, clientSocket);
  }
  return status;
}

//-----------------------------------------------------------------------------

opStatus database::readAllValues(int clientSocket) {
  opStatus memtableStatus = memtableWrapperObjRef->getAllValues(clientSocket);
  if(memtableStatus != opStatus::opSuccess) {
    return memtableStatus;
  }
  return memtableStatus;
  //opStatus sstableStatus = 
  //return sstableStatus
}

//----------------------------------------------------------------------------

database::~database() {
}

//-----------------------------------------------------------------------------
