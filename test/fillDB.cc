#include <fstream>
#include <vector>
#include <string>

static const char alphanum[] =
  "0123456789"
  "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
  "abcdefghijklmnopqrstuvwxyz";


int main() {
  std::ofstream file;
  file.open("input.txt");
  std::string key, value;
  std::vector<unsigned char> buffer;
  for(int i = 0; i < 1000000; ++i) {
    key = "";
    value = "";
    for(int j = 0; j < 10; j++) {
      key += alphanum[rand() % (sizeof(alphanum) - 1)];
      value += alphanum[rand() % (sizeof(alphanum) - 1)];
    }
    buffer.clear();
    buffer.push_back((unsigned char)key.length());
    std::copy(key.begin(), key.end(), std::back_inserter(buffer));
    buffer.push_back((unsigned char)value.length());
    std::copy(value.begin(), value.end(), std::back_inserter(buffer));
    for(int i = 0; i < (int)buffer.size(); i++) {
      file << buffer[i];
    }
  }
  file.close();
}
