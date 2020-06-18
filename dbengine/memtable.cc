#include "memtable.h"
#include "../serverCode/server.h"

memtable::memtable(int size, int capacity, int memtableID)
  :size(size),
  capacity(capacity),
  memtableID(memtableID) {

}

//-----------------------------------------------------------------------------

std::string memtable::putKeyValuePair(keyValuePair_t keyValuePair) {
  table[keyValuePair.key] = keyValuePair.value;
  ++size;
  return "Inserted successfully!";
}

//-----------------------------------------------------------------------------

bool memtable::isKeyPresent(std::string key) {
  if(table.find(key) != table.end()) {
    return true;
  }
  return false;
}

//-----------------------------------------------------------------------------

void memtable::getValueFromKey(std::string key,int clientSocket) {
  sendToClient(clientSocket, (char)table[key].length());
  sendToClient(clientSocket, table[key]);
}

//-----------------------------------------------------------------------------

void memtable::getAllValues(int clientSocket) {
  std::map<std::string, std::string>::iterator it;
  for(it=table.begin();it!=table.end();it++){
    std::string ret = it->first+" -------> "+it->second+"\n";
    sendToClient(clientSocket, (char)ret.length());
    sendToClient(clientSocket, ret);
  }
}

//------------------------------------------------------------------------------

memtable::~memtable() {

}

//-----------------------------------------------------------------------------
