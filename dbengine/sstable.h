#ifndef SSTABLE_H
#define SSTABLE_H

#include <fstream>

#include "memtable.h"

class sstable {
  public:
    // Constructor
    sstable();
    // To dump a memtable in SSTable
    void dumpMemtableToSSTable(memtable* memtableObjRef);
    // To check if a key is present in SSTable
    bool isKeyPresent(std::string key);
    // To get the key's value
    std::string getValueFromKey(std::string key);
    // Destructor
    ~sstable();
  
  private:
    // To open the file, also creates a file if not already present.
    void openFile();
    // To close the file
    void closeFile();
    // Path to the file for the SSTable file
    std::string filePath;
    // File descriptor
    std::ofstream fd;
    // Total bytes in the SSTable file
    int no_bytes;
};

#endif
