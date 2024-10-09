#include <iostream>

#include "../src/archive.hpp"

int main() {
  ssak::archive_itr itr("test.tar.gz");
  for (const auto& [fname, byte_str] : itr) {
    std::cout << fname << std::endl;
    std::cout << byte_str;
  }
}