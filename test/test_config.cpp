#include "test_main.hpp"

#include "config.hpp"
#include "util/tmpfiles.hpp"

static const char *conf1 =
"[test]\n"
"hello=world\n"
"abc =  123\n";

int test_config() {
  ssak::util::tmpdir tmp_dir("test_config");
  tmp_dir << fs::path("conf1.conf") << conf1;
  ssak::Config c(tmp_dir.get_file_path("conf1.conf"));
  assert(c["test.abc"] == std::string("123"));
  assert(c["test.hello"] == std::string("world"));
  return 0;
}