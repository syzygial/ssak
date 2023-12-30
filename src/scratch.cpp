#include <cstdio>
#include <cstdlib>
#include <sstream>
#include <unistd.h>

#include "ssak.hpp"

namespace ssak {

static const ssak_tool scratch;

void scratch_help(std::ostream &o) {
  std::stringstream s;
  s << "Usage: ssak scratch\n";
  o << s.str() << std::endl;
}

int ssak_scratch(int argc, char **argv) {
  return 0;
}

int ssak_scratch_list(int argc, char **argv) {
  return 0;
}

int ssak_scratch_archive(std::string exp_name) {
  return 0;
}

int ssak_scratch_archive_cmd(int argc, char **argv) {
  return 0;
}

} // namespace ssak