#ifndef DB_H
#define DB_H

#include "keyValuePairStruct.h"
#include "memtableWrapper.h"
#include "utils.h"

class database {
  public:
    // Constructor
    database();
    // Write in DB
    std::string writeKeyValuePair(keyValuePair_t keyValuePair);
    // Read from DB
    std::string readValueFromKey(std::string key);
    // Destructor
    ~database();
  private:
    // MemtableWrapper object
    memtableWrapper* memtableWrapperObjRef;
};

#endif
