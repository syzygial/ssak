#include <iostream>

#include "../util.hpp"
#include "../verbs.hpp"

int main_verb(std::map<ssak::arg_parser::key_type, ssak::arg_parser::value_type>& parsed_args) {
  return 0;
}

int main(int argc, char** argv) {

  ssak::arg_parser main_parser("ssak");
  main_parser.add_argument<std::string_view>(std::nullopt, "abc", std::nullopt, 1, true, ssak::STORE);
  main_parser.add_argument<std::string_view>("-a", std::nullopt, std::nullopt, 1, false, ssak::STORE);
  ssak::verb_tree verbs({"ssak", main_verb, main_parser});
  main_parser.help_message(std::cout);
  //return verbs.parse(argc, argv);
}