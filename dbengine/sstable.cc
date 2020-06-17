#include "sstable.h"

sstable::sstable(std::string filePath)
  : filePath(filePath),
    no_bytes(0){

}

//-----------------------------------------------------------------------------

void sstable::openFileToWrite() {
  writefd.open(filePath);
}

//-----------------------------------------------------------------------------

void sstable::writeInFile(std::vector<unsigned char>& buffer) {
  for(int i = 0; i < buffer.size(); ++i) {
    writefd << buffer[i];
  }
  no_bytes += buffer.size();
}

//----------------------------------------------------------------------------

void sstable::closeFileAfterWrite() {
  std::cout << "Written " 
            << no_bytes 
            << " bytes to the SSTable" 
            << std::endl;
  writefd.close();
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
bool sstable::isKeyPresent(std::string key) {
  //TODO: Use bloom filter
  return false;
}

//-----------------------------------------------------------------------------

std::string sstable::getValueFromKey(std::string key) {
  //TODO: Check if present. Then use in-memory index for offset.
  std::ifstream readfd;
  openFileToRead(readfd);
  unsigned char ch;
  while(readfd.peek() != EOF) {
    readfd >> ch;
    std::string data((int)ch, '\0');
    readfd.read(&data[0], (int)ch);
    std::cout << "Read key : " << data << " from SSTable!" << std::endl;
    readfd >> ch;
    if(data == key) {
      std::string val((int)ch, '\0');
      readfd.read(&val[0], (int)ch);
      return val;
    }
    readfd.seekg(ch, std::ios::cur);
  }
  closeFileAfterRead(readfd);
  return "Key not found!";
}

//-----------------------------------------------------------------------------

sstable::~sstable() {

}

//-----------------------------------------------------------------------------
