#include <iostream>

#include "../util.hpp"
#include "../verbs.hpp"

#include "ssak_scratch.hpp"
#include "ssak_task.hpp"

int main_verb(std::map<ssak::arg_parser::key_type, ssak::arg_parser::value_type>& parsed_args) {
  static const std::string commands_str =
  "ssak\n\n"
  "commands:\n"
  "  scratch: experiment scratchpad\n"
  "  task: task information";
  std::cout << commands_str << std::endl;
  return 0;
}

int main(int argc, char** argv) {

  // need this for parsing the verb tree
  if (std::string_view(argv[0]) != "ssak") argv[0] = (char*)"ssak";

  ssak::arg_parser main_parser("ssak");
  ssak::verb_tree verbs({"ssak", main_verb, main_parser});

  auto& ssak_root = verbs.get_root();
  ssak::scratch_init(ssak_root);
  ssak::task_init(ssak_root);

  return verbs.parse(argc, argv);
}