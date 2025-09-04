#ifndef SSAK_ARGS_HPP
#define SSAK_ARGS_HPP

#include <any>
#include <map>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <typeinfo>
#include <tuple>
#include <type_traits>
#include <variant>
#include <vector>

namespace ssak {

namespace detail {

template<typename T>
class is_vector : std::false_type {};

template<typename T>
class is_vector<std::vector<T> > : std::true_type {
  public:
  using vec_type = T;
};

template<typename T>
constexpr bool is_vector_v = is_vector<T>::value;

template<typename... T>
class is_variant : std::false_type {};

template<typename... T>
class is_variant<std::variant<T...> > : std::true_type {
  public:
  using variant_types = std::tuple<T...>;
};

template<typename... T>
constexpr bool is_variant_v = is_variant<T...>::value;

template<typename T>
class is_any : std::false_type {};

template<>
class is_any<std::any> : std::true_type {};

template<typename T>
constexpr bool is_any_v = is_any<T>::value;

template<typename var_type, typename T, int I>
constexpr int variant_index_by_type(std::integer_sequence<int, I> seq) {
  if (std::is_same_v<std::variant_alternative_t<I, var_type>, T>) return I;
   else return -1;
}

template<typename var_type, typename T, int I, int... Ints>
constexpr int variant_index_by_type(std::integer_sequence<int, I, Ints...> seq) {
  if (!std::is_same_v<std::variant_alternative_t<I, var_type>, T>) {
    return variant_index_by_type<var_type, T, Ints...>(std::integer_sequence<int, Ints...>{});
  }
  return I;
}

template<typename var_type, typename T>
constexpr int variant_index_by_type() {
  return variant_index_by_type<var_type, T>(std::make_integer_sequence<int, std::variant_size_v<var_type> >{});
}

template<typename var_type, typename T>
constexpr int variant_index_by_type_v = variant_index_by_type<var_type, T>();

} // namespace detail

class bad_argument : public std::runtime_error {

public:
bad_argument() : std::runtime_error(default_what) {}
bad_argument(std::string what_arg) : std::runtime_error(what_arg) {}
private:
static constexpr char default_what[] = "Ill-formed command-line argument spec";

};

typedef enum {
  STORE=1,
  STORE_TRUE,
  STORE_FALSE
} arg_action;

class arg_parser {
  public:
  using nargs_type = std::variant<char*,int>;
  using key_type = std::string;
  using value_type = std::variant<bool,char*,int,std::vector<char*>,std::vector<int> >;
  //using value_type = std::variant<char*,int>;
  arg_parser() {}

  template<typename T>
  void add_argument(std::optional<std::string> short_name,
      std::optional<std::string> long_name,
      std::optional<std::string> dest_name,
      nargs_type nargs,
      bool required,
      arg_action action) {
    if (!this->valid_arg(short_name, long_name, dest_name, nargs)) throw bad_argument();
    if (short_name == std::nullopt) short_name = std::string();
    if (long_name == std::nullopt) long_name = std::string();
    if (dest_name == std::nullopt) {
      if (long_name.value().empty()) dest_name = short_name;
      else dest_name = long_name;
    }
    auto dest_start = dest_name.value().begin();
    while ((*dest_start) == '-') dest_start++;
    dest_name = std::string(dest_start, dest_name.value().end());
    const int arg_index = detail::variant_index_by_type_v<value_type, T>;

    argument new_arg(short_name.value(), long_name.value(), dest_name.value(), nargs, required, action, arg_index);
    //auto new_arg = std::unique_ptr<const argument>(new const argument(short_name.value(), long_name.value(), dest_name.value(), nargs, required, action, arg_index));
    this->arguments.push_back(new_arg);
  }
  std::map<key_type, value_type> parse_args(int argc, char* const* argv) {
    std::map<std::string, value_type> parsed_args;
    this->initialize_args(parsed_args);
    for (auto& a : this->arguments) {
      unsigned int matched_args = this->match(a, argv);
      if (matched_args == 0) continue;
      this->parse_arg(parsed_args, a, argv, matched_args);
      //if ()

      argc -= matched_args;
      argv += matched_args;
    }
    return parsed_args;
  }

  private:
  struct argument {
    argument(std::string short_name, std::string long_name, std::string dest_name, nargs_type nargs, bool required, arg_action action, const int var_index) : 
      short_name(short_name),
      long_name(long_name),
      dest_name(dest_name),
      nargs(nargs),
      action(action),
      required(required),
      var_index(var_index) {
        this->positional = !(this->long_name.starts_with("-"));
      }
    std::string short_name;
    std::string long_name;
    std::string dest_name;
    nargs_type nargs;
    bool required;
    bool positional;
    arg_action action;
    const int var_index;
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
  bool valid_arg(std::optional<std::string> short_name, std::optional<std::string> long_name, std::optional<std::string> dest_name, nargs_type nargs) {
    // TODO: check validity of parameters
    return true;
  }
  void initialize_args(std::map<key_type, value_type>& args) {
    // TODO: kind of greasy but it is what it is.. maybe there is a template metaprogramming way to do this.
    // basically, initialize based on add_argument template parameter
    for (auto& a: this->arguments) {
      if (a.var_index == 0) { // bool
        args[a.dest_name] = bool();
      }
      else if (a.var_index == 1) { // char*
        args[a.dest_name] = (char*)nullptr;
      }
      else if (a.var_index == 2) { // int
        args[a.dest_name] = int();
      }
      else if (a.var_index == 3) { // std::vector<char*>
        args[a.dest_name] = std::vector<char*>{};
      }
      else if (a.var_index == 4) { // std::vector<int>
        args[a.dest_name] = std::vector<int>{};
      }
      else { // bad variant

      }
      //args[a.dest_name] = std::variant_alternative_t<(a.var_index), value_type>();
      //args[a.dest_name] = std::variant_alternative_t<(get_arg_var_index(a)), value_type>();
    }
  }
  void parse_arg(std::map<key_type, value_type>& args_map, const argument& matched_arg, char* const* argv, int args_matched) {
    if (matched_arg.action == STORE_TRUE) args_map[matched_arg.dest_name] = true;
    else if (matched_arg.action == STORE_FALSE) args_map[matched_arg.dest_name] = false;
    else if (matched_arg.action == STORE) args_map[matched_arg.dest_name] = this->convert_arg(matched_arg, *argv);
  }
  value_type convert_arg(const argument& arg_spec, const char* arg) {
    // TODO: write arg conversion function
    return (char*)nullptr;
  }
  std::vector<argument> arguments;
};

} // namespace ssak

#endif