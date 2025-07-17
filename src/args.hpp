#ifndef SSAK_ARGS_HPP
#define SSAK_ARGS_HPP

#include <map>
#include <optional>
#include <string>
#include <string_view>
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
void add_argument(std::optional<std::string> short_name,
    std::optional<std::string> long_name,
    std::optional<std::string> dest_name,
    value_type nargs,
    const std::type_info& arg_type) {
  if (!this->valid_arg(short_name, long_name, dest_name, nargs, arg_type)) return;
  if (short_name == std::nullopt) short_name = std::string();
  if (long_name == std::nullopt) long_name = std::string();
  if (dest_name == std::nullopt) {
    if (long_name.value().empty()) dest_name = short_name;
    else dest_name = long_name;
  }
  auto dest_start = dest_name.value().begin();
  while ((*dest_start) == '-') dest_start++;
  dest_name = std::string(dest_start, dest_name.value().end());
  argument new_arg(short_name.value(), long_name.value(), dest_name.value(), nargs, arg_type);
  this->arguments.push_back(new_arg);
}
std::map<key_type, value_type> parse_args(int argc, char* const* argv) {
  std::map<std::string, value_type> parsed_args;
  for (auto a : this->arguments) {
    if (this->match(a, argv) == 0) continue;
  }
  return parsed_args;
}

private:
struct argument {
  argument(std::string short_name, std::string long_name, std::string dest_name, value_type nargs, const std::type_info& arg_type) : 
    short_name(short_name),
    long_name(long_name),
    dest_name(dest_name),
    nargs(nargs),
    arg_type(arg_type) {}
  std::string short_name;
  std::string long_name;
  std::string dest_name;
  std::string dest;
  value_type nargs;
  const std::type_info& arg_type;
};
unsigned int match(const argument& arg, char* const* argv) {
  //if (arg.short_name.starts_with("--"));
  #include <iostream>
  if (!arg.long_name.empty() && (std::string_view(*argv)).starts_with(arg.long_name)) {
    std::cout << ":)" << std::endl;
  }
  else if (arg.short_name.starts_with("-"));
  else;
  return 0;
}
bool valid_arg(std::optional<std::string> short_name, std::optional<std::string> long_name, std::optional<std::string> dest_name, value_type nargs, const std::type_info& arg_type) {
  // TODO: check validity of parameters
  return true;
}
std::vector<argument> arguments;
};

} // namespace ssak

#endif