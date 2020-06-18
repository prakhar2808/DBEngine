#ifndef DB_H
#define DB_H

#include <pthread.h>

#include "operation.h"
#include "keyValuePairStruct.h"
#include "memtableWrapper.h"
#include "sstableWrapper.h"
#include "utils.h"

class database {
  public:
    // Constructor
    database();
    // Write in DB
    opStatus writeKeyValuePair(keyValuePair_t keyValuePair, int clientSocket);
    // Read from DB
    opStatus readValueFromKey(std::string key, int clientSocket);
    // Read all from DB
    opStatus readAllValues(int clientSocket);
    // Destructor
    ~database();
  private:
    // MemtableWrapper object
    memtableWrapper* memtableWrapperObjRef;
    // SSTable object
    sstableWrapper* sstableWrapperObjRef;
};

#endif
