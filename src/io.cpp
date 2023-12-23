#include "ssak.hpp"

#include <algorithm>
#include <array>
#include <cstdio>
#include <iostream>
#include <streambuf>
#include <unistd.h>

namespace ssak {

bool is_printable(const std::string &s) {
  
}

std::string sanitize(const std::string &s) {

}

class ansi_printer {
public:
  ansi_printer();
  ansi_printer(FILE *fp);
  bool isatty();
  int flush() { return std::fflush(this->fp); }
  ansi_printer& operator<<(std::string &s);
private:
  std::FILE *fp;
  std::array<bool, 3> is_std;
  friend std::ostream& operator<<(std::ostream &o, const ansi_printer &ap);
};

ansi_printer::ansi_printer(){
  this->fp = NULL;
}

ansi_printer& ansi_printer::operator<<(std::string &s) {
  if (this->isatty()) {

  }
  return *this;
}

ansi_printer::ansi_printer(FILE *fp) {
  this->fp = fp;
  if (fileno(fp) < 3 && fileno(fp) > -1) is_std[fileno(fp)] = true;
} 

std::ostream& operator<<(std::ostream &o, const ansi_printer &ap) {
  // check if the ostream is connected to stdout 
  if (o.rdbuf() == std::cout.rdbuf() && ::isatty(STDOUT_FILENO)) {}
  return o;
}

bool ansi_printer::isatty() {
  return ::isatty(fileno(this->fp));
}
} // namespace ssak