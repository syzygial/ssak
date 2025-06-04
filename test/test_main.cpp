#include "test_main.hpp"

int main() {

  REGISTER_TEST_SQLITE;
  REGISTER_TEST_ARCHIVE;
  REGISTER_TEST_TOOLS_SCRATCH;
  
  auto harness = TestHarness::inst();
  return harness->run_tests();
}