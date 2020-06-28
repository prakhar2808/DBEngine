#ifndef KEYVALUEPAIRSTRUCT_H
#define KEYVALUEPAIRSTRUCT_H

#include <string>

struct keyValuePair_t {
  std::string key;
  std::string value;
  keyValuePair_t() {
    key = "";
    value = "";
  }
  keyValuePair_t(std::string k, std::string v) {
    key = k;
    value = v;
  }
};


#endif
