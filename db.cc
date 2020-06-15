#include "db.h"

database::database()
  : memtableWrapperObjRef(new memtableWrapper()){

}

//-----------------------------------------------------------------------------

void database::writeKeyValuePair(keyValuePair_t keyValuePair) {
  memtableWrapperObjRef->putKeyValuePair(keyValuePair);
}

//-----------------------------------------------------------------------------

void database::readValueFromKey(std::string key) {
  std::cout << memtableWrapperObjRef->getValueFromKey(key)
            << std::endl;
}

//-----------------------------------------------------------------------------

database::~database() {
}

//-----------------------------------------------------------------------------
