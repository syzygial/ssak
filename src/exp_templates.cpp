#include <filesystem>
#include <fstream>
#include <map>
#include <vector>

#include "exp_templates.hpp"

static const project_template c_project {
  {"test.c", 
"int main() {\
  return 0;\
}"}
};

static const std::map<const char*, project_template> templates {
  {"c", c_project}
};

void initialize_template(const char* template_name, const fs::path &root) {
  project_template t = templates.at(template_name);
  for (auto f : t) {
    for (const auto& [name, contents] : f) {
      const fs::path p(std::string(root) + "/" + std::string(name));
      if (!fs::exists(p.parent_path())) fs::create_directories(p.parent_path());
      std::ofstream s(p);
      s << contents;
    }
  }
}