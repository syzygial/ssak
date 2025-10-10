#ifndef SSAK_SCRATCH_HPP
#define SSAK_SCRATCH_HPP

#include "../args.hpp"
#include "../verbs.hpp"

namespace ssak {
  arg_parser ssak_scratch_p("ssak scratch");
  int scratch_verb_fn(std::map<ssak::arg_parser::key_type, ssak::arg_parser::value_type>& parsed_args) {
    return 0;
  }
  verb ssak_scratch("scratch", scratch_verb_fn, ssak_scratch_p);
}

#endif