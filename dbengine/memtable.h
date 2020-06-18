#ifndef MEMTABLE_H
#define MEMTABLE_H


#include <map>

#include "keyValuePairStruct.h"
#include "utils.h"

class memtable {
  public:
    // Data members
    int size, capacity;
    // Constructor
    memtable(int size, int capacity);
    // Put a key, value pair in the memtable
    std::string putKeyValuePair(keyValuePair_t keyValuePair);
    // Check if a key is present in the memtable
    bool isKeyPresent(std::string key);
    // Retrieve a value from the memtable for a given key
    void getValueFromKey(std::string key, int clientSocket);
    // Get all values from memtable
    void getAllValues(int clientSocket);
    // Destructor
    ~memtable();
  private:
    std::map<std::string, std::string> table;
};

#endif
