#include "bloomFilter.h"
#include "MurmurHash3.h"
#include <sys/stat.h>

bloomFilter::bloomFilter(std::string bloomFilePath)
  : bloomFilePath(bloomFilePath){
  //Load file into bitset if the bloom file exists.
  struct stat buff;
  if(stat(bloomFilePath.c_str(), &buff) != -1) {
    std::ifstream bloomFile(bloomFilePath);
    // Load the file into the string
    std::string tempStr((std::istreambuf_iterator<char>(bloomFile)),
                        std::istreambuf_iterator<char>());  
    filterArr = std::bitset<FILTER_SIZE>(tempStr);
    bloomFile.close();
  }
}

//-----------------------------------------------------------------------------
std::array<uint64_t, 2> bloomFilter::hash(std::string data) {
  std::array<uint64_t, 2> hashValue;
  MurmurHash3_x64_128(data.c_str(), int(data.length()), 0, hashValue.data());
  return hashValue;
}

//-----------------------------------------------------------------------------

uint64_t bloomFilter::nthHash(uint8_t n,
                        uint64_t hashA,
                        uint64_t hashB) {
    return (hashA + n * hashB) % FILTER_SIZE;
}

//-----------------------------------------------------------------------------

void bloomFilter::add(std::string data) {
  auto hashValues = hash(data);

  for (int n = 0; n < NUM_HASHES; n++) {
      filterArr[nthHash(n, hashValues[0], hashValues[1])] = true;
  }
}

//-----------------------------------------------------------------------------

void bloomFilter::writeToFile() {
  // Dump to disk.
	std::ofstream ofs(bloomFilePath);
  if (ofs) {
      ofs << filterArr;
  }
  ofs.close();
}

//-----------------------------------------------------------------------------

bool bloomFilter::possiblyContains(std::string data) {
  auto hashValues = hash(data);
  for (int n = 0; n < NUM_HASHES; n++) {
      if (!filterArr[nthHash(n, hashValues[0], hashValues[1])]) {
          return false;
      }
  }
  return true;
}

//-----------------------------------------------------------------------------
