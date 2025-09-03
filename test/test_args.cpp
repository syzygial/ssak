#include "test_main.hpp"

#include "args.hpp"

int test_args() {
  ssak::arg_parser p;
  p.add_argument("-a", std::nullopt, std::nullopt, 0, typeid(std::string), false, ssak::STORE);
  p.add_argument("-t", "--test", std::nullopt, (char*)"+", typeid(int), false, ssak::STORE);
  char* const argv1[] = {(char*)"--test", NULL};
  constexpr int argc1 = (sizeof(argv1) / sizeof(char*)) - 1;
  p.parse_args(argc1, argv1);
  return 0;
}