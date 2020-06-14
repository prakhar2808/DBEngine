#include "memtable.h"

memtable::memtable(int size, int capacity) {
  this->size = size;
  this->capacity = capacity;
}

//-----------------------------------------------------------------------------

void memtable::putKeyValuePair(keyValuePair_t keyValuePair) {
  table[keyValuePair.key] = keyValuePair.value;
}

//-----------------------------------------------------------------------------

bool memtable::isKeyPresent(std::string key) {
  if(table.find(key) != table.end()) {
    return true;
  }
  return false;
}

//-----------------------------------------------------------------------------

std::string memtable::getValueFromKey(std::string key) {
  return table[key];
}

//-----------------------------------------------------------------------------

memtable::~memtable() {

}

//-----------------------------------------------------------------------------
