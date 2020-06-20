#ifndef BLOOMFILTER_H
#define BLOOMFILTER_H

#include <fstream>
#include <vector>

#include "operation.h"
#include "memtable.h"

#define NUM_HASHES 7
#define FILTER_SIZE 1000

class bloomFilter {
  public:
    // Constructor
    bloomFilter(std::string bloomFilePath);
    // Bloom filter
    std::bitset<FILTER_SIZE> filterArr;
    // Path to the file for the bloom filter for the SSTable
    std::string bloomFilePath;
    //Compute two 64 bit Murmur hash values
    std::array<uint64_t, 2> hash(std::string data);
    //Compute the nth hash function
    uint64_t nthHash(uint8_t n, uint64_t hashA, uint64_t hashB);
    //Add data to bllom filter
    void add(std::string data);
    //Write filterArr to file
    void writeToFile();
    //
    bool possiblyContains(std::string data);
    // Destructor
    ~bloomFilter();


};

#endif
