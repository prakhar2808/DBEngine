#include "db.h"
#include "../serverCode/server.h"
#include <chrono>

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

  auto t_start = std::chrono::high_resolution_clock::now();

  opStatus status = memtableWrapperObjRef->getValueFromKey(key, clientSocket);
  if(status == opStatus::opFail) {
    // Check SSTables
    status = sstableWrapperObjRef->getValueFromKey(key, clientSocket);
  }

  auto t_end = std::chrono::high_resolution_clock::now();
  double time_elapsed_ms =
    std::chrono::duration<double, std::milli>(t_end - t_start).count();
  std::cout << "Get operation performed in "
            << time_elapsed_ms
            << " ms!"
            << std::endl;
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
}

//----------------------------------------------------------------------------

database::~database() {
}

//-----------------------------------------------------------------------------
