#include "db.h"

database::database()
  : memtableWrapperObjRef(new memtableWrapper()){

}

//-----------------------------------------------------------------------------

std::string database::writeKeyValuePair(keyValuePair_t keyValuePair) {
  return memtableWrapperObjRef->putKeyValuePair(keyValuePair);
}

//-----------------------------------------------------------------------------

std::string database::readValueFromKey(std::string key) {
  return memtableWrapperObjRef->getValueFromKey(key);
}

//-----------------------------------------------------------------------------

database::~database() {
}

//-----------------------------------------------------------------------------
