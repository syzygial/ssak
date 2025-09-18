#include "../util.hpp"
#include "../verbs.hpp"

int main_verb(std::map<ssak::arg_parser::key_type, ssak::arg_parser::value_type>& parsed_args) {
  return 0;
}

int main(int argc, char** argv) {

  ssak::arg_parser main_parser;

  ssak::verb_tree verbs({"ssak", main_verb, main_parser});
  return verbs.parse(argc, argv);
}