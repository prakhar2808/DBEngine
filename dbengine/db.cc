#include "db.h"
#include "../serverCode/server.h"

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
  //List from SSTables
  opStatus status = sstableWrapperObjRef->getAllValues(clientSocket);
  //Sending end message after fetching from memTables and SSTables
  sendEndMsgToClient(clientSocket);

  return memtableStatus;
  //opStatus sstableStatus = 
  //return sstableStatus
}

//----------------------------------------------------------------------------

database::~database() {
}

//-----------------------------------------------------------------------------
