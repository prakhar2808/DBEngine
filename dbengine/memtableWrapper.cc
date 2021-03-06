#include "memtableWrapper.h"
#include "../serverCode/server.h"

memtableWrapper::memtableWrapper(int lastMemtableID)
  : writeMutex(PTHREAD_MUTEX_INITIALIZER) {
  // Inserting atleast one memtableObject
  memtable* memtableObjPointer = new memtable(0, 
                                              MAX_MEMTABLE_SIZE, 
                                              lastMemtableID);
  memtableObjPointersList.push_back(memtableObjPointer);
  this->lastMemtableID = lastMemtableID;
  index = memtableObjPointersList.begin();
}

//-----------------------------------------------------------------------------

opStatus memtableWrapper::putKeyValuePair(keyValuePair_t keyValuePair, 
                                          sstableWrapper* sstableWrapperObjRef,
                                          int clientSocket) {
  // Locking to write
  pthread_mutex_lock(&writeMutex);
  // Put into the memtable
  (*index)->putKeyValuePair(keyValuePair);
  // Check if currect memtable is full
  if((*index)->size >= (*index)->capacity) {
    // New memtable
    memtable* nextMemtableObjPointer = new memtable(0, 
                                                    MAX_MEMTABLE_SIZE,
                                                    lastMemtableID + 1);
    memtableObjPointersList.push_back(nextMemtableObjPointer);
    ++lastMemtableID;
    
    // An async task to put memtable at index to disk and delete entry
    // from list.
    std::thread(writeMemtableToDisk, 
                index, 
                sstableWrapperObjRef, 
                this).detach();
    
    // Make index point to the next memtable.
    ++index;
    std::cout << "Memtable full, switching to next!" << std::endl;
  }
  // Unlocking
  pthread_mutex_unlock(&writeMutex);
  std::string ret = "Inserted successfully!";
  // Check if it was a put operation corresponding to a deletion.
  if(keyValuePair.value == "") {
    ret = "Deleted the key from the database!";
  }
  sendToClient(clientSocket, ret);
  sendEndMsgToClient(clientSocket);
  return opStatus::opSuccess;
}

//-----------------------------------------------------------------------------

opStatus memtableWrapper::putKeyValuePair(
    std::vector<keyValuePair_t>& keyValuePairVec, 
    sstableWrapper* sstableWrapperObjRef,
    int clientSocket) {
  // Locking to write
  pthread_mutex_lock(&writeMutex);
  // Iterating over the vector and writing.
  for(int i = 0; i < (int)keyValuePairVec.size(); i++) {
    // Put into the memtable
    (*index)->putKeyValuePair(keyValuePairVec[i]);
    // Check if currect memtable is full
    if((*index)->size == (*index)->capacity) {
      std::cout << "Creating new memtable! " << i << std::endl; 
      // New memtable
      memtable* nextMemtableObjPointer = new memtable(0, 
                                                      MAX_MEMTABLE_SIZE,
                                                      lastMemtableID + 1);
      memtableObjPointersList.push_back(nextMemtableObjPointer);
      ++lastMemtableID;
    
      // An async task to put memtable at index to disk and delete entry
      // from list.
      std::thread(writeMemtableToDisk, 
                  index, 
                  sstableWrapperObjRef, 
                  this).detach();
    
      // Make index point to the next memtable.
      ++index;
      std::cout << "Memtable full, switching to next!" << std::endl;
    }
  }
  // Unlocking
  pthread_mutex_unlock(&writeMutex);
  std::string ret = 
    "Inserted " + std::to_string(keyValuePairVec.size()) + " key-value pairs!";
  sendToClient(clientSocket, ret);
  sendEndMsgToClient(clientSocket);
  return opStatus::opSuccess;
}

//-----------------------------------------------------------------------------

opStatus memtableWrapper::getValueFromKey(std::string key, int clientSocket) {
  std::list<memtable*>::iterator iter;
  for(iter = index; iter != memtableObjPointersList.begin(); iter--) {
    if((*iter)->isKeyPresent(key)) {
      std::string val = (*iter)->getValueFromKey(key);
      if(val == "") {
        return opStatus::opKeyNotFound;
      }
      sendToClient(clientSocket, val);
      sendEndMsgToClient(clientSocket);
      return opStatus::opSuccess;
    }
  }

  // Checking in the memtable at the head of the list.
  if((*iter)->isKeyPresent(key)) {
    std::string val = (*iter)->getValueFromKey(key);
    if(val == "") {
      return opStatus::opKeyNotFound;
    }
    sendToClient(clientSocket, val);
    sendEndMsgToClient(clientSocket);
    return opStatus::opSuccess;
  }
  
  return opStatus::opFail;
}

//-----------------------------------------------------------------------------


opStatus memtableWrapper::getAllValues(int clientSocket) {
  std::list<memtable*>::iterator iter;
  for(iter = index; iter != memtableObjPointersList.begin(); iter--) {
    (*iter)->getAllValues(clientSocket);
  }
  // Checking in the memtable at the head of the list.
  (*iter)->getAllValues(clientSocket);
  return opStatus::opSuccess;
}

//-----------------------------------------------------------------------------

void memtableWrapper::writeMemtableToDisk(std::list<memtable*>::iterator iter,
    sstableWrapper* sstableWrapperObjRef, memtableWrapper* thisObj) {
  std::cout << "Dumping memtable "
            << "ID: " << (*iter)->memtableID
            << " to disk..." << std::endl;
  sstableWrapperObjRef->dumpMemtableToSSTable(*iter);
  //std::this_thread::sleep_for(std::chrono::seconds(20));
  //Delete index from list.
  memtable* memtableObj = *iter;
  thisObj->memtableObjPointersList.erase(iter);
  // Erase the journal file and then delete the object
  std::remove(memtableObj->journalFilePath.c_str());
  delete memtableObj;
  std::cout << "Dumped memtable." << std::endl;
}

//-----------------------------------------------------------------------------

memtableWrapper::~memtableWrapper() {
  std::list<memtable*>::iterator iter;
  for(iter = memtableObjPointersList.begin(); 
      iter != memtableObjPointersList.end();
      ++iter) {
    delete *iter;
  }
  memtableObjPointersList.clear();
}

//-----------------------------------------------------------------------------
