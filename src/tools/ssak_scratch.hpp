#ifndef SSAK_TOOLS_SCRATCH_HPP
#define SSAK_TOOLS_SCRATCH_HPP

#include "../args.hpp"
#include "../exp_templates.hpp"
#include "../verbs.hpp"
#include "../scratch/scratch.hpp"

namespace ssak {
  static inline int scratch_verb_fn(std::map<ssak::arg_parser::key_type, ssak::arg_parser::value_type>& parsed_args) {
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
  static inline int scratch_add_verb_fn(std::map<ssak::arg_parser::key_type, ssak::arg_parser::value_type>& parsed_args) {
    return 0;
  }
  static inline int scratch_archive_verb_fn(std::map<ssak::arg_parser::key_type, ssak::arg_parser::value_type>& parsed_args) {
    return 0;
  }
  static inline int scratch_create_verb_fn(std::map<ssak::arg_parser::key_type, ssak::arg_parser::value_type>& parsed_args) {
    std::string_view exp_name = std::get<std::string_view>(parsed_args["exp_name"]);
    std::string_view exp_path = std::get<std::string_view>(parsed_args["exp_path"]);
    std::string_view exp_template = std::get<std::string_view>(parsed_args["template"]);
    ssak::scratch::scratch s;
    if (exp_path.empty()) {
      //s.create_exp(exp_name.data());
    }
    else {
      //s.create_exp(exp_name.data(), exp_path);
    }
    if (!exp_template.empty()) {
      //s.apply_template(exp_name.data(), exp_template.data());
    }
    return 0;
  }
  static inline int scratch_delete_verb_fn(std::map<ssak::arg_parser::key_type, ssak::arg_parser::value_type>& parsed_args) {
    return 0;
  }
  static inline int scratch_list_verb_fn(std::map<ssak::arg_parser::key_type, ssak::arg_parser::value_type>& parsed_args) {
    return 0;
  }
  static inline int scratch_restore_verb_fn(std::map<ssak::arg_parser::key_type, ssak::arg_parser::value_type>& parsed_args) {
    return 0;
  }
  static inline int scratch_templates_verb_fn(std::map<ssak::arg_parser::key_type, ssak::arg_parser::value_type>& parsed_args) {
    std::cout << "Available templates:\n" << std::endl;
    list_templates();
    return 0;
  }
  static inline void scratch_init(ssak::verb_tree::node& ssak_root) {
    // ssak scratch
    arg_parser ssak_scratch_p("ssak scratch");
    verb ssak_scratch("scratch", scratch_verb_fn, ssak_scratch_p);

    // ssak scratch add
    arg_parser ssak_scratch_add_p("ssak scratch add");
    verb ssak_scratch_add("add", scratch_add_verb_fn, ssak_scratch_add_p);

    // ssak scratch archive
    arg_parser ssak_scratch_archive_p("ssak scratch archive");
    verb ssak_scratch_archive("archive", scratch_archive_verb_fn, ssak_scratch_archive_p);

    // ssak scratch create
    arg_parser ssak_scratch_create_p("ssak scratch create");
    ssak_scratch_create_p.add_argument<std::string_view>(std::nullopt, "--template", std::nullopt, 1, false, ssak::STORE);
    ssak_scratch_create_p.add_argument<std::string_view>(std::nullopt, "exp_name", std::nullopt, 1, true, ssak::STORE);
    ssak_scratch_create_p.add_argument<std::string_view>(std::nullopt, "exp_path", std::nullopt, 1, false, ssak::STORE);
    verb ssak_scratch_create("create", scratch_create_verb_fn, ssak_scratch_create_p);

    // ssak scratch delete
    arg_parser ssak_scratch_delete_p("ssak scratch delete");
    verb ssak_scratch_delete("delete", scratch_delete_verb_fn, ssak_scratch_delete_p);

    // ssak scratch list
    arg_parser ssak_scratch_list_p("ssak scratch list");
    verb ssak_scratch_list("list", scratch_list_verb_fn, ssak_scratch_list_p);

    // ssak scratch restore
    arg_parser ssak_scratch_restore_p("ssak scratch restore");
    verb ssak_scratch_restore("restore", scratch_restore_verb_fn, ssak_scratch_restore_p);

    // ssak scratch templates
    arg_parser ssak_scratch_templates_p("ssak scratch templates");
    verb ssak_scratch_templates("templates", scratch_templates_verb_fn, ssak_scratch_templates_p);

  auto& ssak_scratch_node = ssak_root.add_child(ssak_scratch);
  auto& ssak_scratch_add_node = ssak_scratch_node.add_child(ssak_scratch_add);
  auto& ssak_scratch_archive_node = ssak_scratch_node.add_child(ssak_scratch_archive);
  auto& ssak_scratch_create_node = ssak_scratch_node.add_child(ssak_scratch_create);
  auto& ssak_scratch_delete_node = ssak_scratch_node.add_child(ssak_scratch_delete);
  auto& ssak_scratch_list_node = ssak_scratch_node.add_child(ssak_scratch_list);
  auto& ssak_scratch_restore_node = ssak_scratch_node.add_child(ssak_scratch_restore);
  auto& ssak_scratch_templates_node = ssak_scratch_node.add_child(ssak_scratch_templates);
  }
}

#endif