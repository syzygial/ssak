#ifndef SSAK_TOOLS_SCRATCH_HPP
#define SSAK_TOOLS_SCRATCH_HPP

#include "../args.hpp"
#include "../exp_templates.hpp"
#include "../verbs.hpp"
#include "../scratch/scratch.hpp"

namespace ssak {
  // ssak scratch
  arg_parser ssak_scratch_p("ssak scratch");
  int scratch_verb_fn(std::map<ssak::arg_parser::key_type, ssak::arg_parser::value_type>& parsed_args) {
    static const std::string commands_str =
    "ssak scratch - experiment scratchpad\n\n"
    "commands:\n"
    "  add\n"
    "  archive\n"
    "  create\n"
    "  delete\n"
    "  list\n"
    "  restore\n"
    "  templates";
    std::cout << commands_str << std::endl;
    return 0;
  }
  verb ssak_scratch("scratch", scratch_verb_fn, ssak_scratch_p);

  // ssak scratch add
  arg_parser ssak_scratch_add_p("ssak scratch add");
  int scratch_add_verb_fn(std::map<ssak::arg_parser::key_type, ssak::arg_parser::value_type>& parsed_args) {
    return 0;
  }
  verb ssak_scratch_add("add", scratch_add_verb_fn, ssak_scratch_add_p);

  // ssak scratch archive
  arg_parser ssak_scratch_archive_p("ssak scratch archive");
  int scratch_archive_verb_fn(std::map<ssak::arg_parser::key_type, ssak::arg_parser::value_type>& parsed_args) {
    return 0;
  }
  verb ssak_scratch_archive("archive", scratch_archive_verb_fn, ssak_scratch_archive_p);

  // ssak scratch create
  arg_parser ssak_scratch_create_p("ssak scratch create");
  int scratch_create_verb_fn(std::map<ssak::arg_parser::key_type, ssak::arg_parser::value_type>& parsed_args) {
    return 0;
  }
  verb ssak_scratch_create("create", scratch_create_verb_fn, ssak_scratch_create_p);

  // ssak scratch delete
  arg_parser ssak_scratch_delete_p("ssak scratch delete");
  int scratch_delete_verb_fn(std::map<ssak::arg_parser::key_type, ssak::arg_parser::value_type>& parsed_args) {
    return 0;
  }
  verb ssak_scratch_delete("delete", scratch_delete_verb_fn, ssak_scratch_delete_p);

  // ssak scratch list
  arg_parser ssak_scratch_list_p("ssak scratch list");
  int scratch_list_verb_fn(std::map<ssak::arg_parser::key_type, ssak::arg_parser::value_type>& parsed_args) {
    return 0;
  }
  verb ssak_scratch_list("list", scratch_list_verb_fn, ssak_scratch_list_p);

  // ssak scratch restore
  arg_parser ssak_scratch_restore_p("ssak scratch restore");
  int scratch_restore_verb_fn(std::map<ssak::arg_parser::key_type, ssak::arg_parser::value_type>& parsed_args) {
    return 0;
  }
  verb ssak_scratch_restore("restore", scratch_restore_verb_fn, ssak_scratch_restore_p);

  // ssak scratch templates
  arg_parser ssak_scratch_templates_p("ssak scratch templates");
  int scratch_templates_verb_fn(std::map<ssak::arg_parser::key_type, ssak::arg_parser::value_type>& parsed_args) {
    std::cout << "Available templates:\n" << std::endl;
    list_templates();
    return 0;
  }
  verb ssak_scratch_templates("templates", scratch_templates_verb_fn, ssak_scratch_templates_p);
}

#endif