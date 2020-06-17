#include "sstable.h"

sstable::sstable() {
  filePath = "";
  //fd = NULL;
  no_bytes = 0;
}

//-----------------------------------------------------------------------------

void sstable::dumpMemtableToSSTable(memtable* memtableObjRef) {

}

//-----------------------------------------------------------------------------

bool sstable::isKeyPresent(std::string key) {
  return false;
}

//-----------------------------------------------------------------------------

std::string sstable::getValueFromKey(std::string key) {
  return "";
}

//-----------------------------------------------------------------------------

sstable::~sstable() {

}

//-----------------------------------------------------------------------------

void sstable::openFile() {
  fd.open(filePath);
}

//-----------------------------------------------------------------------------

void sstable::closeFile() {
  fd.close();
}

//-----------------------------------------------------------------------------
