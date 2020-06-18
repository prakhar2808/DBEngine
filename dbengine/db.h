#ifndef DB_H
#define DB_H

#include <pthread.h>

#include "keyValuePairStruct.h"
#include "memtableWrapper.h"
#include "utils.h"

class database {
  public:
    // Constructor
    database();
    // Write in DB
    void writeKeyValuePair(keyValuePair_t keyValuePair, int clientSocket);
    // Read from DB
    void readValueFromKey(std::string key, int clientSocket);
    // Read all from DB
    void readAllValues(int clientSocket);
    // Destructor
    ~database();
  private:
    // MemtableWrapper object
    memtableWrapper* memtableWrapperObjRef;
};

#endif
