#include "test_main.hpp"

#include "util.hpp"

int test_util() {
  ssak::tree<int> t(3);
  auto& root = t.get_root();
  root.add_child(4);
  root.add_child(5);
  root.add_sibling(6);
  return 0;
}