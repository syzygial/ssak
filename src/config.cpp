#include <map>

#include "ssak.hpp"

namespace ssak {

class config {
public:
  config(std::FILE *f);
private:
  std::map<std::string, std::string> params;
};

} // namespace ssak