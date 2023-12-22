#include "ssak.hpp"

#include <streambuf>
#include <unistd.h>

namespace ssak {

class ansi_printer {
public:
  ansi_printer(FILE *fp) : fp(fp) {}
  bool isatty();
private:
  std::FILE *fp;
};

bool ansi_printer::isatty() {
  return ::isatty(fileno(this->fp));
}

} // namespace ssak