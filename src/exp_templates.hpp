#ifndef SSAK_EXP_TEMPLATES_HPP
#define SSAK_EXP_TEMPLATES_HPP


#include <filesystem>

namespace fs = std::filesystem;

typedef std::vector<std::map<char*,char*> > project_template;

void initialize_template(const char* template_name, const fs::path &root);

#endif