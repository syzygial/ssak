#include "../test_main.hpp"

#include "util/tmpfiles.hpp"

int test_util_tmpfiles() {
  ssak::util::tmpdir tmp_dir("test_tmpfiles");
  tmp_dir << fs::path("a.txt") << "Hello world\n";
  tmp_dir << fs::path("a/b/c.txt") << "abc" << "123\n";
  return 0;
}