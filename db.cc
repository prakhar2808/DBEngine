#include "db.h"

database::database()
  : memtableObj(0, MAX_MEMTABLE_SIZE){
}

//-----------------------------------------------------------------------------

void database::writeKeyValuePair(keyValuePair_t keyValuePair) {
  memtableObj.putKeyValuePair(keyValuePair);
  ++memtableObj.size;
}

//-----------------------------------------------------------------------------

void database::readValueFromKey(std::string key) {
  if(memtableObj.isKeyPresent(key)) {
    std::cout << "Value: " 
              << memtableObj.getValueFromKey(key)
              << std::endl;
  }
  else {
    std::cout << "ERROR: Invalid Key. "
              << "No record found for the given key." 
              << std::endl;
  }
}

//-----------------------------------------------------------------------------

database::~database() {
}

//-----------------------------------------------------------------------------
