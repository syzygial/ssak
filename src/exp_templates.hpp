#ifndef SSAK_EXP_TEMPLATES_HPP
#define SSAK_EXP_TEMPLATES_HPP

#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

namespace ssak {

typedef std::vector<std::pair<const char*, const char*> > project_template;

void initialize_template(const char* template_name, const fs::path &root);

void list_templates();
void list_templates(std::ostream& o);

} // namespace ssak

#endif