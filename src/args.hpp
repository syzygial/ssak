#ifndef SSAK_ARGS_HPP
#define SSAK_ARGS_HPP

#include <map>
#include <string>
#include <typeinfo>
#include <variant>
#include <vector>

namespace ssak {

template
<typename key_type=std::string, typename value_type=std::variant<char*,int> >
class arg_parser {
public:
//using value_type = std::variant<char*,int>;
arg_parser() {}
void add_argument(const char *arg_name, std::type_info& arg_type) {
  argument new_arg(arg_type);
  this->arguments.push_back(new_arg);
}
std::map<std::string, value_type> parse_args(int argc, char* const* argv) {
  std::map<std::string, value_type> parsed_args;
  for (auto a : this->arguments) {

  }
  return parsed_args;
}

private:
struct argument {
  argument(std::type_info& arg_type) : arg_type(arg_type) {}
  char *short_name;
  char *long_name;
  char *dest;
  std::type_info& arg_type;
};
std::vector<argument> arguments;
};

} // namespace ssak

#endif