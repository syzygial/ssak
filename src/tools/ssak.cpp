#include <iostream>

#include "../util.hpp"
#include "../verbs.hpp"

#include "ssak_scratch.hpp"

int main_verb(std::map<ssak::arg_parser::key_type, ssak::arg_parser::value_type>& parsed_args) {
  static const std::string commands_str = 
  "commands:\n\n"
  "ssak\n"
  "  scratch: experiment scratchpad";
  std::cout << commands_str << std::endl;
  return 0;
}

int main(int argc, char** argv) {

  // need this for parsing the verb tree
  if (std::string_view(argv[0]) != "ssak") argv[0] = (char*)"ssak";

  ssak::arg_parser main_parser("ssak");
  ssak::verb_tree verbs({"ssak", main_verb, main_parser});

  auto& ssak_root = verbs.get_root();

  auto& ssak_scratch = ssak_root.add_child(ssak::ssak_scratch);
  auto& ssak_scratch_add = ssak_scratch.add_child(ssak::ssak_scratch_add);
  auto& ssak_scratch_archive = ssak_scratch.add_child(ssak::ssak_scratch_archive);
  auto& ssak_scratch_create = ssak_scratch.add_child(ssak::ssak_scratch_create);
  auto& ssak_scratch_delete = ssak_scratch.add_child(ssak::ssak_scratch_delete);
  auto& ssak_scratch_list = ssak_scratch.add_child(ssak::ssak_scratch_list);
  auto& ssak_scratch_restore = ssak_scratch.add_child(ssak::ssak_scratch_restore);
  return verbs.parse(argc, argv);
}