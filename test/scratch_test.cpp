#include "../src/scratch/scratch.hpp"

int main() {
  ssak::scratch::scratch s;
  s.add_exp("test_exp", fs::path("a"));
  s.archive_exp("test_exp");
}