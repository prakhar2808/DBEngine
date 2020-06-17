#ifndef MEMTABLE_WRAPPER_H
#define MEMTABLE_WRAPPER_H

#include "keyValuePairStruct.h"
#include "memtable.h"
#include "sstableWrapper.h"

#include <future>
#include <list>
#include <pthread.h>

#define MAX_MEMTABLE_SIZE 2

class memtableWrapper {
  public:
    // Constructor
    memtableWrapper(int lastMemtableID);
    // Put a key-value pair in memtable
    std::string putKeyValuePair(keyValuePair_t keyValuePair,
                                sstableWrapper* sstableWrapperObjRef);
    // Get a value for a key from the memtable
    std::string getValueFromKey(std::string key);
    // Write memtable to disk
    static void writeMemtableToDisk(std::list<memtable*>::iterator iter,
                                    sstableWrapper* sstableWrapperObjRef);
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
    // Write lock to the memtables.
    pthread_mutex_t writeMutex;
    // ID of last created memtable
    int lastMemtableID;
};

#endif
