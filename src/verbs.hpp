#ifndef SSAK_VERBS_HPP
#define SSAK_VERBS_HPP

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
      /*try {
        auto parsed_args = parser.parse_args(argc, argv);
        return verb_fn(parsed_args);
      }
      catch(const bad_argument& e) {
        std::cerr << "error: " <<  e.what() << std::endl;
        parser.help_message(std::cerr);
        exit(1);
      }*/
      auto parsed_args = parser.parse_args(argc, argv);
      return verb_fn(parsed_args);
    }
    const std::string_view get_name() const {
      return this->name;
    }
    private:
    ssak::arg_parser parser;
    std::string name;
    fn_type verb_fn;
  };

  class verb_tree : public tree<verb> {
    public:
    verb_tree(verb v) : tree<verb>(v) {}
    int add_verb(const std::string_view& verb_name, verb::fn_type verb_fn) {
      return 0;
    }
    int parse(int argc, char** argv) {
      verb& v = this->get_verb(&argc, &argv);
      return v.run(argc, argv);
    }

    private:
    verb& get_verb(int *argc, char*** argv) {
      verb *v = nullptr;
      auto& root_verb = this->get_root();
      auto* verb_itr = &root_verb;
      auto sibling_itr = (*verb_itr).sibling_itr();
      while (!sibling_itr.done() && *argc > 0) {
        if ((**sibling_itr).get_name() == **argv) {
          v = &(**sibling_itr);
          if (!(*sibling_itr).is_leaf()) {
            verb_itr = &(verb_itr->get_first_child());
            sibling_itr = verb_itr->sibling_itr();
          }
          (*argc)--;
          (*argv)++;
          continue;
        }
        sibling_itr++;
      }
        //break; // no matching subcommand
      //if (verb_itr == &root_verb) {
      //  throw std::runtime_error("could not find verb");
      //}
      //return **verb_itr;
      return *v;
    }
  };
} // namespace ssak

#endif