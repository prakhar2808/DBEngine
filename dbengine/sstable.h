#ifndef SSTABLE_H
#define SSTABLE_H

#include <fstream>
#include <vector>
#include <map>

#include "operation.h"
#include "memtable.h"
#include "bloomFilter.h"

#define FS_BLOCK_SIZE 1024

class sstable {
  public:
    // Constructor
    sstable(std::string filePath, 
            std::string bloomFilePath,
            std::string indexFilePath = "");
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
    // In-memory index - mapping from key to offset in the file.
    std::map<std::string, long long> index; 
    // To load the index in memory
    void loadIndexInMemory();
    // List all key-value pairs
    opStatus getAllValues(int clientSocket);
    //Bloom Filter class
    bloomFilter* filter;
    // Destructor
    ~sstable();

  
  private:
    // To open the file to read
    void openFileToRead(std::ifstream& readfd);
    // To close the file after read
    void closeFileAfterRead(std::ifstream& readfd);
    // Path to the file for the SSTable file
    std::string filePath;
    // File descriptor for writing SSTable
    std::ofstream writefd;
    // File descriptor for writing index
    std::ofstream writeIndexfd; 
    // Total bytes in the SSTable file
    long long no_bytes;
    // Total blocks written in the SSTable file
    int total_blocks;
};

#endif
