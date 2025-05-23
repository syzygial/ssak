#include "test_main.hpp"

int main() {

  REGISTER_TEST_SQLITE;

  auto harness = TestHarness::inst();
  return harness->run_tests();
}