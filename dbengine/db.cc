#include "db.h"

database::database() {
  sstableWrapperObjRef = new sstableWrapper();
  memtableWrapperObjRef = 
    new memtableWrapper(*(sstableWrapperObjRef->sstable_no));
}

//-----------------------------------------------------------------------------

std::string database::writeKeyValuePair(keyValuePair_t keyValuePair) {
  std::string ret = memtableWrapperObjRef->putKeyValuePair(keyValuePair,
    sstableWrapperObjRef);
  return ret;
}

//-----------------------------------------------------------------------------

std::string database::readValueFromKey(std::string key) {
  //return memtableWrapperObjRef->getValueFromKey(key);
  return sstableWrapperObjRef->getValueFromKey(key);
}

//-----------------------------------------------------------------------------

database::~database() {
}

//-----------------------------------------------------------------------------
