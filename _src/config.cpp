#include <map>

#include "ssak.hpp"

namespace ssak {

class config {
public:
  config(std::FILE *f);
  std::string operator[](std::string param) { return this->params[param]; }
private:
  std::map<std::string, std::string> params;
};

} // namespace ssak