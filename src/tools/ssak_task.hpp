#ifndef SSAK_TOOLS_TASK_HPP
#define SSAK_TOOLS_TASK_HPP

#include <iostream>

#include "../args.hpp"
#include "../verbs.hpp"
#include "../task/task.hpp"

namespace ssak {
  static inline int task_verb_fn(std::map<ssak::arg_parser::key_type, ssak::arg_parser::value_type>& parsed_args) {
    static const std::string commands_str = 
      "ssak task - task information\n\n"
      "commands:\n"
      "  graph\n"
      "  list\n"
      "  popq\n"
      "  pushq\n";
    std::cout << commands_str << std::endl;
    return 0;
  }
  static inline void task_init(ssak::verb_tree::node& ssak_root) {
    // ssak task
    arg_parser ssak_task_p("ssak task");
    verb ssak_task("task", task_verb_fn, ssak_task_p);

    auto& ssak_task_node = ssak_root.add_child(ssak_task);
  }
} // namespace ssak

#endif