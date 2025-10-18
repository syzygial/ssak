#include <filesystem>
#include <fstream>
#include <map>
#include <vector>

#include "exp_templates.hpp"

namespace ssak {

static const project_template c_project {
  {"test.c", 
  "int main() {\n"
  "  return 0;\n"
  "}\n"
}
};

static const std::map<std::string, project_template> templates {
  {"c", c_project}
};

void initialize_template(const char* template_name, const fs::path &root) {
  project_template t = templates.at(template_name);
  for (auto& [name, contents] : t) {
    const fs::path p(std::string(root) + "/" + std::string(name));
    if (!fs::exists(p.parent_path())) fs::create_directories(p.parent_path());
    std::ofstream s(p);
    s << contents;
  }
}

void list_templates() {
  list_templates(std::cout);
}

void list_templates(std::ostream& o) {
  for (auto& [k,v] : templates) {
    std::cout << k << std::endl;
  }
}

} // namespace ssak