#include "test_main.hpp"

int main() {

  REGISTER_TESTS;
  
  auto harness = TestHarness::inst();
  return harness->run_tests();
}