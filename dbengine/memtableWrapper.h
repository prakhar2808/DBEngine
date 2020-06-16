#ifndef MEMTABLE_WRAPPER_H
#define MEMTABLE_WRAPPER_H

#include "keyValuePairStruct.h"
#include "memtable.h"

#include <list>

#define MAX_MEMTABLE_SIZE 10

class memtableWrapper {
  public:
    // Constructor
    memtableWrapper();
    // Put a key-value pair in memtable
    std::string putKeyValuePair(keyValuePair_t keyValuePair);
    // Get a value for a key from the memtable
    std::string getValueFromKey(std::string key);
    // Destructor
    ~memtableWrapper();
  private:
    // A list to store the list of pointers to all the memtable objects
    // present in the memory. A list is maintained in order to satisfy the read
    // request during the time when a memtable is being written to the disk as
    // an SSTable.
    std::list<memtable*> memtableObjPointersList;
    // Iterator to the memtableObjPointer which handles incoming write request.
    std::list<memtable*>::iterator index;
};

#endif
