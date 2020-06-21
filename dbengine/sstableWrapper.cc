#include "sstableWrapper.h"
#include "sstable.h"
#include "../serverCode/server.h"

sstableWrapper::sstableWrapper() {
  int i = 0;
  while(true) {
    if(!SSTableExists("SSTablesDir/" + std::to_string(i) + ".txt")) {
      break;
    }
    sstable* sstableObjRef = new sstable(
        "SSTablesDir/" + std::to_string(i) + ".txt",
        "bloomFiltersDir/" + std::to_string(i) + ".txt",
        "SSTablesDir/" + std::to_string(i) + ".txt.index");
    sstableObjRefList.push_back(sstableObjRef);
    ++i;
  }
  sstable_no = (int*)malloc(sizeof(int));
  *sstable_no = i;
  std::cout << "Loaded " << i << " SSTables from the disk!" << std::endl;
}

//-----------------------------------------------------------------------------

void sstableWrapper::dumpMemtableToSSTable(memtable* memtableObjRef) {
  // File path
  std::string filePath = 
    "SSTablesDir/" + std::to_string(memtableObjRef->memtableID) + ".txt";
  // Bloom File path
  std::string bloomFilePath =
    "bloomFiltersDir/" + std::to_string(i) + ".txt";
  // Creating SSTable
  sstable* sstableObjRef = new sstable(filePath, bloomFilePath);
  // Opening SSTable file to write
  sstableObjRef->openFileToWrite();
  // Writing to the file
  std::map<std::string, std::string>::iterator iter;
  // Bytes array
  std::vector<unsigned char> buffer;
  // To write each key-value pair in the SSTable and index using buffer.
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
    // Add the record to file and index.
    sstableObjRef->writeInFile(buffer);
    // Add the key to bloom filter.
    sstableObjRef->filter->add(iter->first);
  }
  
  // Close the file to save the changes. SSTable and index files are dumped.
  sstableObjRef->closeFileAfterWrite();
  // Dump the bloom filter to the disk.
  sstableObjRef->filter->writeToFile();
  
  // Loading the index in memory.
  sstableObjRef->loadIndexInMemory();
  
  // Inserting SSTable to list.
  sstableObjRefList.push_back(sstableObjRef);
  // Increasing the count of number of SSTables.
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
    // Check the in-memory bloom filter first.
    if(!((*iter)->isKeyPresent(key))) {
      continue;
    }
    // Now the SSTable may contain the key.
    opStatus ret = (*iter)->getValueFromKey(key, clientSocket);
    // The key is found in the SSTable then the return will be opSuccess.
    if(ret == opStatus::opSuccess) {
      std::cout << "Checked " << count << " SSTables and found!" << std::endl;
      return ret;
    }
  }
  std::cout << "Checked " << count << " SSTables but NOT found!" << std::endl;
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

//-----------------------------------------------------------------------------

bool sstableWrapper::SSTableExists(std::string filePath) {
  struct stat buff;
  if(stat(filePath.c_str(), &buff) == -1) {
    return false;
  }
  return true;
}

//-----------------------------------------------------------------------------
