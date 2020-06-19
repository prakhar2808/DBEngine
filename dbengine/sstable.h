#ifndef SSTABLE_H
#define SSTABLE_H

#include <fstream>
#include <vector>

#include "operation.h"
#include "memtable.h"

class sstable {
  public:
    // Constructor
    sstable(std::string filePath);
    // To open the file to write, also creates a file if not already present.
    void openFileToWrite();
    // To write in the file
    void writeInFile(std::vector<unsigned char>& buffer);
    // To close the file after write
    void closeFileAfterWrite();
    // To check if a key is present in SSTable
    bool isKeyPresent(std::string key);
    // To get the key's value
    opStatus getValueFromKey(std::string key, int clientSocket);
    // List all key-value pairs
    opStatus getAllValues(int clientSocket);
    // Destructor
    ~sstable();
  
  private:
    // To open the file to read
    void openFileToRead(std::ifstream& readfd);
    // To close the file after read
    void closeFileAfterRead(std::ifstream& readfd);
    // Path to the file for the SSTable file
    std::string filePath;
    // File descriptor for writing
    std::ofstream writefd;
    // Total bytes in the SSTable file
    int no_bytes;
};

#endif
