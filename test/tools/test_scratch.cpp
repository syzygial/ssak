#include "../test_main.hpp"

#include "scratch/scratch.hpp"

int test_ssak_scratch() {
  fs::remove("test.db");
  auto scratch = ssak::scratch::scratch("test.db");
  return 0;
}