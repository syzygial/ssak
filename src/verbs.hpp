#ifndef SSAK_VERBS_HPP
#define sSAK_VERBS_HPP

#include "args.hpp"

namespace ssak {
  class verb {
    public:
    using fn_type = int(*)(std::map<ssak::arg_parser::key_type, ssak::arg_parser::value_type>&);
    int run(int argc, char** argv) {
      auto parsed_args = p.parse_args(argc, argv);
      return verb_fn(parsed_args);
    }

    private:
    ssak::arg_parser p;
    fn_type verb_fn;
  };
}

#endif