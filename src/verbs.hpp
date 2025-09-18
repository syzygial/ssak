#ifndef SSAK_VERBS_HPP
#define sSAK_VERBS_HPP

#include "args.hpp"
#include "util.hpp"

namespace ssak {
  class verb {
    public:
    using fn_type = int(*)(std::map<ssak::arg_parser::key_type, ssak::arg_parser::value_type>&);
    verb(std::string name, fn_type verb_fn, ssak::arg_parser parser) : name(name), verb_fn(verb_fn), parser(parser) {}
    verb(std::tuple<std::string, fn_type, ssak::arg_parser> verb_args) {
      auto& [name, verb_fn, parser] = verb_args;
      verb(name, verb_fn, parser);
    }
    int run(int argc, char** argv) {
      auto parsed_args = parser.parse_args(argc, argv);
      return verb_fn(parsed_args);
    }

    private:
    ssak::arg_parser parser;
    std::string name;
    fn_type verb_fn;
  };

  class verb_tree : public tree<verb> {
    public:
    verb_tree(verb v) : tree<verb>(v) {}
    verb::fn_type get_verb_fn() {
      return nullptr;
    }
    int parse(int argc, char** argv) {
      return 0;
    }
  };
} // namespace ssak

#endif