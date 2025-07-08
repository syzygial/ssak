#include "test_main.hpp"

#include "args.hpp"

int test_args() {
  ssak::arg_parser p;
  p.add_argument("-a", std::nullopt, std::nullopt, 0, typeid(std::string));
  p.add_argument("-t", "--test", std::nullopt, (char*)"+", typeid(int));
  return 0;
}