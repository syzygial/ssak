#include <cstdlib>
#include <filesystem>
#include <format>
#include <fstream>
#include <map>
#include <vector>

#include "exp_templates.hpp"

namespace ssak {

// This section generated automatically by scripts/gen_project_templates.py

static const project_template c_project {
  {"main.c",
  "int main() {\n"
  "  return 0;\n"
  "}\n"
  },
};
static const project_template pulseaudio_project {
  {"CMakeLists.txt",
  "cmake_minimum_required(VERSION 3.0)\n"
  "\n"
  "project(pulse_example VERSION 0.1.0)\n"
  "\n"
  "find_package(PulseAudio REQUIRED)\n"
  "\n"
  "add_executable(pulse_example pulse_example.c)\n"
  "target_link_libraries(pulse_example pulse)\n"
  },
  {"pulse_example.c",
  "#include <pulse/pulseaudio.h>\n"
  "\n"
  "int main() {\n"
  "  pa_threaded_mainloop *m = pa_threaded_mainloop_new();\n"
  "  pa_mainloop_api *m_api = pa_threaded_mainloop_get_api(m);\n"
  "\n"
  "  pa_threaded_mainloop_stop(m);\n"
  "  pa_threaded_mainloop_free(m);\n"
  "  return 0;\n"
  "}\n"
  },
};
static const project_template py_package_project {};

static const project_template qt6_project {};

static const project_template win32_project {};

static const project_template opengl_glfw_project {};

static const project_template x11_project {};

static const project_template cmake_project {};

static const project_template gtk_project {};

static const project_template directx9_project {};

static const std::map<std::string, project_template> templates {
  {"c", c_project},
  {"pulseaudio", pulseaudio_project},
  {"py_package", py_package_project},
  {"qt6", qt6_project},
  {"win32", win32_project},
  {"opengl_glfw", opengl_glfw_project},
  {"x11", x11_project},
  {"cmake", cmake_project},
  {"gtk", gtk_project},
  {"directx9", directx9_project},
};

// End automatically-generated section

void initialize_template(const char* template_name, const fs::path &root) {
  const project_template *t = nullptr;
  if (std::getenv("SSAK_DEBUG") == nullptr) {
    try {
      t = &(templates.at(template_name));
    }
    catch (std::out_of_range& e) {
      std::cout << std::format("Template {} does not exist, skipping...", template_name) << std::endl;
      return;
    }
  }
  else {
    t = &(templates.at(template_name));
  }
  for (auto& [name, contents] : *t) {
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