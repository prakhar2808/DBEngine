#ifndef DB_H
#define DB_H

#include <pthread.h>

#include "keyValuePairStruct.h"
#include "memtableWrapper.h"
#include "sstableWrapper.h"
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
    // SSTable object
    sstableWrapper* sstableWrapperObjRef;
};

#endif
