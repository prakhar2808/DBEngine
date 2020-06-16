#ifndef SSTABLE_WRAPPER_H
#define SSTABLE_WRAPPER_H

#include "memtable.h"
#include "pthread.h"
#include "sstable.h"

#include <list>

class sstableWrapper {
  public:
    // Constructor
    sstableWrapper();
    // To dump a memtable in a SSTable
    void dumpMemtableToSSTable(memtable* memtableObjRef);
    // Get key's value from SSTables
    std::string getValueFromKey(std::string key);
    // Destructor
    ~sstable();

  private:
    // List of pointers to all the SSTable objects.
    // Note : Insert the pointer in the list only after the SSTable has been
    // created and completely written to and before the memtable 
    // has been deleted.
    std::list<sstable*> sstableObjRefList;
    // Total SSTables created till now
    int* sstable_no;
    // Mutex to use while creating a new unique SSTable
    pthread_mutex_t sstableCreatemutex;
};

#endif
