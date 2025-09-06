#ifndef SSAK_ARGS_HPP
#define SSAK_ARGS_HPP

#include <any>
#include <cstring>
#include <cstdlib>
#include <map>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <typeinfo>
#include <tuple>
#include <type_traits>
#include <utility>
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

  template<typename T>
  class valid_arg_type : public std::false_type {};

  template<>
  class valid_arg_type<bool> : public std::true_type{};

  template<>
  class valid_arg_type<int> : public std::true_type{};

  template<>
  class valid_arg_type<char*> : public std::true_type{};

  template<typename T>
  constexpr bool valid_arg_type_v = valid_arg_type<T>::value;

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
  using nargs_type = std::variant<int,std::pair<int,int> >;
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
    if (!detail::valid_arg_type_v<T>) throw bad_argument{};
    const int max_nargs = this->max_nargs(nargs);
    const int arg_index = (max_nargs > 1 || max_nargs == -1) ?
      detail::variant_index_by_type_v<value_type, T> + 2 : // std::vector<T>
      detail::variant_index_by_type_v<value_type, T>; // T

    argument new_arg(short_name.value(), long_name.value(), dest_name.value(), nargs, required, action, arg_index);
    //auto new_arg = std::unique_ptr<const argument>(new const argument(short_name.value(), long_name.value(), dest_name.value(), nargs, required, action, arg_index));
    this->arguments.push_back(new_arg);
  }
  std::map<key_type, value_type> parse_args(int argc, char* const* argv) {
    std::map<std::string, value_type> parsed_args;
    this->initialize_args(parsed_args);
    for (auto& a : this->arguments) {
      unsigned int matched_args = this->match(a, argv);
      if (matched_args == -1) continue;
      this->parse_arg(parsed_args, a, argv, matched_args);
      //if ()

      argc -= (matched_args+1);
      argv += (matched_args+1);
    }
    return parsed_args;
  }

  private:
  using value_tag = enum {BOOL=0, CSTR, INT, CSTR_VEC, INT_VEC};
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

  int match(const argument& arg, char* const* argv) {
    bool match_found = false;
    int matched_args = -1;
    if (!arg.long_name.empty() && (std::string_view(*argv)) == arg.long_name) {
      match_found = true;
    }
    else if (std::string_view(*argv) == arg.short_name) {
      match_found = true;
    }
    if (match_found) {
      argv++;
      matched_args++;
      while (*argv != nullptr && !is_arg(*argv) && valid_nargs(matched_args + 1, arg.nargs, true)) {
        argv++;
        matched_args++;
      }
      if (!valid_nargs(matched_args, arg.nargs, false)) throw bad_argument{};
    }
    return matched_args;
  }
  bool valid_arg(std::optional<std::string> short_name, std::optional<std::string> long_name, std::optional<std::string> dest_name, nargs_type nargs) {
    // TODO: check validity of parameters
    return true;
  }
  void initialize_args(std::map<key_type, value_type>& args) {
    // TODO: kind of greasy but it is what it is.. maybe there is a template metaprogramming way to do this.
    // basically, initialize based on add_argument template parameter
    for (auto& a: this->arguments) {
      if (a.var_index == BOOL) { // bool
        args[a.dest_name] = bool();
      }
      else if (a.var_index == CSTR) { // char*
        args[a.dest_name] = (char*)nullptr;
      }
      else if (a.var_index == INT) { // int
        args[a.dest_name] = int();
      }
      else if (a.var_index == CSTR_VEC) { // std::vector<char*>
        args[a.dest_name] = std::vector<char*>{};
      }
      else if (a.var_index == INT_VEC) { // std::vector<int>
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
  value_type convert_arg(const argument& a, const char* arg) {
    if (a.var_index == CSTR || a.var_index == CSTR_VEC) { // char* || std::vector<char*>

    }
    else if (a.var_index == INT || a.var_index == INT_VEC) { // int || std::vector<int>

    }
    else { // bad variant

    }
    return (char*)nullptr;
  }
  bool is_arg(const std::string_view& s) {
    for (auto& a : this->arguments) {
      if (!a.long_name.empty() && s == a.long_name) return true;
      if (!a.short_name.empty() && s == a.short_name) return true;
    }
    return false;
  }
  bool valid_nargs(int args_matched, const nargs_type& nargs_str, bool counting) {
    if (std::holds_alternative<int>(nargs_str)) {
      if (counting) return args_matched <= std::get<int>(nargs_str);
      else return args_matched == std::get<int>(nargs_str);
    }
    auto [min_nargs, max_nargs] = std::get<std::pair<int,int> >(nargs_str);
    if (min_nargs == -1) {
      return args_matched <= max_nargs;
    }
    else if (max_nargs == -1) {
      if (counting) return true;
      else return args_matched >= min_nargs;
    }
    else {
      if (counting) return args_matched <= max_nargs;
      else return (args_matched >= min_nargs) && (args_matched <= max_nargs);
    }
  }
  int min_nargs(const nargs_type& nargs_str) {
    if (std::holds_alternative<int>(nargs_str)) return std::get<int>(nargs_str);
    else if (std::holds_alternative<std::pair<int, int> >(nargs_str)) return std::get<std::pair<int, int> >(nargs_str).first;
    else throw bad_argument{};
  }
  int max_nargs(const nargs_type& nargs_str) {
    if (std::holds_alternative<int>(nargs_str)) return std::get<int>(nargs_str);
    else if (std::holds_alternative<std::pair<int, int> >(nargs_str)) return std::get<std::pair<int, int> >(nargs_str).second;
    else throw bad_argument{};
  }
  std::vector<argument> arguments;
};

} // namespace ssak

#endif