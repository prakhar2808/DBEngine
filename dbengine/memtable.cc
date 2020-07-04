#include "memtable.h"
#include "../serverCode/server.h"
#include <sys/stat.h>

memtable::memtable(int size, int capacity, int memtableID)
  :size(size),
  capacity(capacity),
  memtableID(memtableID) {
  
  journalFilePath = "journalsDir/" + std::to_string(memtableID) + ".txt";
  // Check if the journal file is already present, need to restore it in memory
  struct stat buff;
  if(stat(journalFilePath.c_str(), &buff) != -1) {
    std::ifstream journalReadfd;
    journalReadfd.open(journalFilePath);
    journalReadfd >> std::noskipws;
    unsigned char ch;
    std::cout << "Reading journal : " << journalFilePath << std::endl;
    while(journalReadfd.peek() != EOF) {
      // Length of the key
      journalReadfd >> ch;
      std::string key((int)ch, '\0');
      // The key
      journalReadfd.read(&key[0], (int)ch);
      // Length of the value
      journalReadfd >> ch;
      std::string value((int)ch, '\0');
      // The value
      journalReadfd.read(&value[0], (int)ch);
      // Putting in the in-memory table.
      table[key] = value;
      std::cout << "Read key : " << key << ", value : " << value << std::endl; 
    }
    journalReadfd.close();
  }
  // Opening the journal in append mode.
  journalWritefd.open(journalFilePath, std::ios_base::app);
}

//-----------------------------------------------------------------------------

void memtable::putKeyValuePair(keyValuePair_t keyValuePair) {
  bool isKeyPresent = this->isKeyPresent(keyValuePair.key);
  table[keyValuePair.key] = keyValuePair.value;
  // Increase size only on addition of a new key, and not on update.
  if(!isKeyPresent) {
    ++size;
  }
  // Log into the journal
  journalWritefd << (unsigned char)(keyValuePair.key.length())
                 << keyValuePair.key.c_str()
                 << (unsigned char)(keyValuePair.value.length())
                 << keyValuePair.value.c_str();
  journalWritefd.flush();
}

//-----------------------------------------------------------------------------

bool memtable::isKeyPresent(std::string key) {
  if(table.find(key) != table.end()) {
    return true;
  }
  return false;
}

//-----------------------------------------------------------------------------

std::string memtable::getValueFromKey(std::string key) {
  return table[key];
}

//-----------------------------------------------------------------------------

void memtable::getAllValues(int clientSocket) {
  std::map<std::string, std::string>::iterator it;
  for(it=table.begin();it!=table.end();it++){
    std::string ret = it->first+" -------> "+it->second+"\n";
    sendToClient(clientSocket, ret);
  }
}

//------------------------------------------------------------------------------

memtable::~memtable() {
  journalWritefd.close();
}

//-----------------------------------------------------------------------------
