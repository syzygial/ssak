#include "test_main.hpp"

#include "args.hpp"

int test_args() {
  ssak::arg_parser p;
  p.add_argument<char*>("-a", std::nullopt, std::nullopt, 0, false, ssak::STORE);
  p.add_argument<int>("-t", "--test", std::nullopt, std::pair<int,int>{1,-1}, false, ssak::STORE);
  char* const argv1[] = {(char*)"--test", (char*)"10", NULL};
  constexpr int argc1 = (sizeof(argv1) / sizeof(char*)) - 1;
  auto parsed_args = p.parse_args(argc1, argv1);
  return 0;
}