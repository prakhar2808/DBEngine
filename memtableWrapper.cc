#include "memtableWrapper.h"

memtableWrapper::memtableWrapper() {
  // Inserting atleast one memtableObject
  memtable* memtableObjPointer = new memtable(0, MAX_MEMTABLE_SIZE);
  memtableObjPointersList.push_back(memtableObjPointer);
  index = memtableObjPointersList.begin();
}

//-----------------------------------------------------------------------------

void memtableWrapper::putKeyValuePair(keyValuePair_t keyValuePair) {
  (*index)->putKeyValuePair(keyValuePair);
}

//-----------------------------------------------------------------------------

std::string memtableWrapper::getValueFromKey(std::string key) {
  std::list<memtable*>::iterator iter;
  for(iter = index; iter != memtableObjPointersList.begin(); iter--) {
    if((*iter)->isKeyPresent(key)) {
      return (*iter)->getValueFromKey(key);
    }
  }
  // Checking in the memtable at the head of the list.
  if((*iter)->isKeyPresent(key)) {
    return (*iter)->getValueFromKey(key);
  }
  return "Error: Key Invalid. Key-Value pair not found in Memtable.";
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
