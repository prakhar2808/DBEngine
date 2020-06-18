#include "memtableWrapper.h"
#include "../serverCode/server.h"

memtableWrapper::memtableWrapper()
  : writeMutex(PTHREAD_MUTEX_INITIALIZER) {
  // Inserting atleast one memtableObject
  memtable* memtableObjPointer = new memtable(0, MAX_MEMTABLE_SIZE);
  memtableObjPointersList.push_back(memtableObjPointer);
  index = memtableObjPointersList.begin();
}

//-----------------------------------------------------------------------------

void memtableWrapper::putKeyValuePair(keyValuePair_t keyValuePair, int clientSocket) {
  // Locking to write
  pthread_mutex_lock(&writeMutex);
  // Put into the memtable
  std::string ret = (*index)->putKeyValuePair(keyValuePair);
  // Check if currect memtable is full
  if((*index)->size == (*index)->capacity) {
    // New memtable
    memtable* nextMemtableObjPointer = new memtable(0, MAX_MEMTABLE_SIZE);
    memtableObjPointersList.push_back(nextMemtableObjPointer);
    
    // An async task to put memtable at index to disk and delete entry
    // from list.
    std::list<memtable*>::iterator index_copy = index;
    //std::future<bool> futRet = 
      //std::async(std::launch::async, writeMemtableToDisk, index_copy);
    std::thread(writeMemtableToDisk, index_copy).detach();
    // Make index point to the newly created memtable.
    ++index;
    std::cout << "Memtable full, switching to next!" << std::endl;
  }
  // Unlocking
  pthread_mutex_unlock(&writeMutex);
  sendToClient(clientSocket, (char)ret.length());
  sendToClient(clientSocket, ret);
  sendToClient(clientSocket, (char)9);
  sendToClient(clientSocket, "```end```");
}

//-----------------------------------------------------------------------------

void memtableWrapper::getValueFromKey(std::string key, int clientSocket) {
  std::list<memtable*>::iterator iter;
  for(iter = index; iter != memtableObjPointersList.begin(); iter--) {
    if((*iter)->isKeyPresent(key)) {
      (*iter)->getValueFromKey(key, clientSocket);
      sendToClient(clientSocket, (char)9);
      sendToClient(clientSocket, "```end```");
      return;
      // return (*iter)->getValueFromKey(key);
    }
  }
  // Checking in the memtable at the head of the list.
  if((*iter)->isKeyPresent(key)) {
    (*iter)->getValueFromKey(key, clientSocket);
    sendToClient(clientSocket, (char)9);
    sendToClient(clientSocket, "```end```");
    return;
  }
  std::string ret="Error: Key Invalid. Key-Value pair not found in Memtable.";
  sendToClient(clientSocket, (char)ret.length());
  sendToClient(clientSocket, ret);
  sendToClient(clientSocket, (char)9);
  sendToClient(clientSocket, "```end```");
  // return "Error: Key Invalid. Key-Value pair not found in Memtable.";
}

//-----------------------------------------------------------------------------


void memtableWrapper::getAllValues(int clientSocket) {
  std::list<memtable*>::iterator iter;
  for(iter = index; iter != memtableObjPointersList.begin(); iter--) {
    (*iter)->getAllValues(clientSocket);
  }
  // Checking in the memtable at the head of the list.
  (*iter)->getAllValues(clientSocket);
  sendToClient(clientSocket, (char)9);
  sendToClient(clientSocket, "```end```");

}

//-----------------------------------------------------------------------------

void memtableWrapper::writeMemtableToDisk
  (std::list<memtable*>::iterator iter) {
  std::cout << "In here to sleep" << std::endl;
  std::this_thread::sleep_for(std::chrono::seconds(20));
  std::cout << "Out of here" << std::endl;
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
