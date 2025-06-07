#include <filesystem>

#include "../test_main.hpp"

#include "scratch/scratch.hpp"
#include "util/tmpfiles.hpp"

namespace fs = std::filesystem;

int test_ssak_scratch() {
  fs::remove("test.db");
  auto scratch = ssak::scratch::scratch("test.db");
  ssak::util::tmpdir tmp_dir("test_archive");
  tmp_dir << fs::path("a.txt") << "hello world\n";
  tmp_dir << fs::path("a/b/c.txt") << "abc123\n";
  scratch.add_exp("test_exp", tmp_dir.get_root_dir());
  scratch.archive_exp("test_exp");
  return 0;
}