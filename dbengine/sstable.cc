#include "sstable.h"
#include "../serverCode/server.h"
#include <sys/stat.h>
#include "bloomFilter.h"

sstable::sstable(std::string filePath, 
                 std::string bloomFilePath,
                 std::string indexFilePath)
  : filePath(filePath),
    no_bytes(0),
    total_blocks(-1){
  
  // Bloom filter object  
  filter = new bloomFilter(bloomFilePath);
  
  struct stat buff;
  // Check if index present
  if(stat(indexFilePath.c_str(), &buff) != -1) {
    loadIndexInMemory();
  }
}

//-----------------------------------------------------------------------------

void sstable::openFileToWrite() {
  writefd.open(filePath);
  writeIndexfd.open(filePath + ".index");
}

//-----------------------------------------------------------------------------

void sstable::writeInFile(std::vector<unsigned char>& buffer) {
  // Writing in SSTable.
  for(int i = 0; i < (int)buffer.size(); ++i) {
    writefd << buffer[i];
  }
  
  // Check if this is the first key in the block, if yes then put in index.
  if((no_bytes / FS_BLOCK_SIZE) > total_blocks) {
    // Length of key
    writeIndexfd << buffer[0];
    // The key
    for(int i = 1; i < (int)buffer[0] + 1; ++i) {
      writeIndexfd << buffer[i];
    }
    long long cpy_no_bytes = no_bytes;
    std::vector<unsigned char> offset;
    while(cpy_no_bytes) {
      offset.push_back((unsigned char)(cpy_no_bytes % 10 + '0'));
      cpy_no_bytes /= 10;
    }
    std::reverse(offset.begin(), offset.end());
    if(no_bytes == 0) {
      offset.push_back('0');
    }
    // Length of the offset's string representation
    writeIndexfd << (unsigned char)(offset.size());
    // The offset's string representation
    for(int i = 0; i < (int)offset.size(); ++i) {
      writeIndexfd << offset[i];
    }
    ++total_blocks;
  }
  
  // Updating total number of bytes written in the SSTable.
  no_bytes += (long long)buffer.size();
}

//----------------------------------------------------------------------------

void sstable::closeFileAfterWrite() {
  writefd.close();
  writeIndexfd.close();
}

//-----------------------------------------------------------------------------

void sstable::openFileToRead(std::ifstream& readfd) {
  readfd.open(filePath);
  // Don't skip whitespace
  readfd >> std::noskipws;
}

//-----------------------------------------------------------------------------

void sstable::closeFileAfterRead(std::ifstream& readfd) {
  readfd.close();
}

//-----------------------------------------------------------------------------

void sstable::loadIndexInMemory() {
  std::ifstream readfd;
  std::string indexFilePath = filePath + ".index";
  readfd.open(indexFilePath);
  readfd >> std::noskipws;
  unsigned char ch;
  long long offset;
  while(readfd.peek() != EOF) {
    // Length of key
    readfd >> ch;
    std::string key((int)ch, '\0');
    // The key
    readfd.read(&key[0], (int)ch);
    std::cout << "Index : Read key " << key;
    // Length of offset
    readfd >> ch;
    offset = 0;
    // The offset
    std::string offsetStr((int)ch, '\0');
    readfd.read(&offsetStr[0], (int)ch);
    // Converting offset to long long from string
    for(int i = 0; i < (int)ch; i++) {
      offset = offset * 10 + (long long)(offsetStr[i] - '0');
    }
    std::cout << " Offset = " << offset << std::endl; 
    index[key] = offset;
  }
  readfd.close();
}

//-----------------------------------------------------------------------------

bool sstable::isKeyPresent(std::string key) {
  return filter->possiblyContains(key);
}

//-----------------------------------------------------------------------------

opStatus sstable::getValueFromKey(std::string key, int clientSocket) {
  // Getting the offset of the block using the index.
  std::map<std::string, long long>::iterator iter = index.lower_bound(key);

  // If key is in the index then the iterator points to that map entry.

  // If key is not in the index then the iterator points to the next higher
  // entry in the map. We need to go back 1 block. If we are on the first block
  // then all the keys in this SSTable are greater than the one we are looking
  // for.
  if(iter->first != key && iter == index.begin()) {
    return opStatus::opFail;
  }
  // If iter points to anywhere else
  else if(iter->first != key) {
    --iter;
  }

  std::ifstream readfd;
  openFileToRead(readfd);
  // Directly going to the block to read.
  readfd.seekg(iter->second, std::ios::cur);

  unsigned char ch;
  while(readfd.peek() != EOF) {
    readfd >> ch;
    std::string data((int)ch, '\0');
    readfd.read(&data[0], (int)ch);
    if(data > key) {
      break;
    }
    readfd >> ch;
    if(data == key) {
      std::string val((int)ch, '\0');
      readfd.read(&val[0], (int)ch);
      // If the value has been deleted.
      if(val == "") {
        return opStatus::opKeyNotFound;
      }
      sendToClient(clientSocket, val);
      sendEndMsgToClient(clientSocket);
      return opStatus::opSuccess;
    }
    readfd.seekg(ch, std::ios::cur);
  }
  closeFileAfterRead(readfd);
  return opStatus::opFail;
}

//-----------------------------------------------------------------------------

opStatus sstable::getAllValues(int clientSocket) {
  std::ifstream readfd;
  openFileToRead(readfd);
  unsigned char ch;
  while(readfd.peek() != EOF) {
    readfd >> ch;
    std::string data((int)ch, '\0');
    readfd.read(&data[0], (int)ch);
    std::cout << "Read key : " << data << " from SSTable!" << std::endl;
    readfd >> ch;
    std::string val((int)ch, '\0');
    readfd.read(&val[0], (int)ch);
    sendToClient(clientSocket, data+" -------> "+val+"\n");
  }
  closeFileAfterRead(readfd);
  return opStatus::opSuccess;
}

//-----------------------------------------------------------------------------

sstable::~sstable() {
  delete filter;
}

//-----------------------------------------------------------------------------
