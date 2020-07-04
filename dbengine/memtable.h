#ifndef MEMTABLE_H
#define MEMTABLE_H

#include <fstream>
#include <map>

#include "operation.h"
#include "keyValuePairStruct.h"
#include "utils.h"

class memtable {
  public:
    std::map<std::string, std::string> table;
    // Data members
    int size, capacity, memtableID;
    // Constructor
    memtable(int size, int capacity, int memtableID);
    // Put a key, value pair in the memtable
    void putKeyValuePair(keyValuePair_t keyValuePair);
    // Check if a key is present in the memtable
    bool isKeyPresent(std::string key);
    // Retrieve a value from the memtable for a given key
    std::string getValueFromKey(std::string key);
    // Get all values from memtable
    void getAllValues(int clientSocket);
    // Destructor
    ~memtable();

  public:
    // Path to the journal file.
    std::string journalFilePath;
    // File descriptor to append to the journal
    std::ofstream journalWritefd;
};

#endif
