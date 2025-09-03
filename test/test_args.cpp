#include "test_main.hpp"

#include "args.hpp"

int test_args() {
  ssak::arg_parser p;
  p.add_argument<std::string>("-a", std::nullopt, std::nullopt, 0, false, ssak::STORE);
  p.add_argument<int>("-t", "--test", std::nullopt, (char*)"+", false, ssak::STORE);
  char* const argv1[] = {(char*)"--test", NULL};
  constexpr int argc1 = (sizeof(argv1) / sizeof(char*)) - 1;
  p.parse_args(argc1, argv1);
  return 0;
}