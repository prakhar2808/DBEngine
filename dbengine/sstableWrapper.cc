#include "sstableWrapper.h"
#include "../serverCode/server.h"

sstableWrapper::sstableWrapper() {
  int i = 0;
  while(true) {
    if(!SSTableExists("SSTablesDir/" + std::to_string(i) + ".txt")) {
      break;
    }
    sstable* sstableObjRef = new sstable(
        "SSTablesDir/" + std::to_string(i) + ".txt",
        "bloomFiltersDir/"+ std::to_string(i) +".txt");

    sstableObjRefList.push_back(sstableObjRef);
    ++i;
  }
  sstable_no = (int*)malloc(sizeof(int));
  *sstable_no = i;
  std::cout << "Loaded " << i << " SSTables from the disk!" << std::endl;
}

//-----------------------------------------------------------------------------

void sstableWrapper::dumpMemtableToSSTable(memtable* memtableObjRef) {
  // Creating SSTable
  sstable* sstableObjRef = new sstable(
      "SSTablesDir/" + std::to_string(memtableObjRef->memtableID) + ".txt",
      "bloomFiltersDir/" + std::to_string(memtableObjRef->memtableID) + ".txt");
  // Opening SSTable file to write
  sstableObjRef->openFileToWrite();
  // Writing to the file
  std::map<std::string, std::string>::iterator iter;
  // Bytes array
  std::vector<unsigned char> buffer;
  // To write each key-value pair in the SSTable using buffer.
  for(iter = memtableObjRef->table.begin();
      iter != memtableObjRef->table.end();
      ++iter) {
    buffer.clear();
    buffer.push_back((iter->first).length());
    std::copy((iter->first).begin(), 
              (iter->first).end(), 
              std::back_inserter(buffer));
    buffer.push_back((iter->second).length());
    std::copy((iter->second).begin(),
              (iter->second).end(),
              std::back_inserter(buffer));
    sstableObjRef->writeInFile(buffer);
    sstableObjRef->filter->add(iter->first);
  }
  // Close the file to save the changes.
  sstableObjRef->closeFileAfterWrite();
  sstableObjRef->filter->writeToFile();
  // Inserting SSTable to list.
  sstableObjRefList.push_back(sstableObjRef);
  ++(*sstable_no);
}

//-----------------------------------------------------------------------------

opStatus sstableWrapper::getValueFromKey(std::string key, int clientSocket) {
  std::list<sstable*>::reverse_iterator iter;
  int count = 0;
  for(iter = sstableObjRefList.rbegin();
      iter != sstableObjRefList.rend();
      ++iter) {
    ++count;
    if(!((*iter)->filter->possiblyContains(key)))continue;
    opStatus ret = (*iter)->getValueFromKey(key, clientSocket);
    if(ret == opStatus::opSuccess) {
      return ret;
    }
  }
  std::cout << "Checked " << count << " SSTables!" << std::endl;
  return opStatus::opFail;
}

//-----------------------------------------------------------------------------

opStatus sstableWrapper::getAllValues(int clientSocket) {
  std::list<sstable*>::reverse_iterator iter;
  int count = 0;
  for(iter = sstableObjRefList.rbegin();
      iter != sstableObjRefList.rend();
      ++iter) {
    ++count;
    opStatus ret = (*iter)->getAllValues(clientSocket);
  }
  std::cout << "Listed " << count << " SSTables!" << std::endl;
  return opStatus::opSuccess;
}

//-----------------------------------------------------------------------------

sstableWrapper::~sstableWrapper() {
  std::list<sstable*>::iterator iter;
  for(iter = sstableObjRefList.begin();
      iter != sstableObjRefList.end();
      ++iter) {
    delete *iter;
  }
  delete sstable_no;
  sstableObjRefList.clear(); 
}

//-------------------------------------------------------

bool sstableWrapper::SSTableExists(std::string filePath) {
  struct stat buff;
  if(stat(filePath.c_str(), &buff) == -1) {
    return false;
  }
  return true;
}

//-----------------------------------------------------------------------------