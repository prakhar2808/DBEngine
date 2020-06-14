#ifndef DB_H
#define DB_H

#include "keyValuePairStruct.h"
#include "memtable.h"
#include "utils.h"

#define MAX_MEMTABLE_SIZE 10

class database {
  public:
    // Constructor
    database();
    // Write in DB
    void writeKeyValuePair(keyValuePair_t keyValuePair);
    // Read from DB
    void readValueFromKey(std::string key);
    // Destructor
    ~database();
  private:
    // Memtable object
    memtable memtableObj;
};

#endif
