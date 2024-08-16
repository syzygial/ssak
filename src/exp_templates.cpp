#ifndef SSAK_EXP_TEMPLATES_HPP
#define SSAK_EXP_TEMPLATES_HPP

#include <map>
#include <vector>

typedef std::vector<std::map<char*,char*> > project_template;

static const project_template c_project {
  {"test.c", 
"int main() {\
  return 0;\
}"}
};

static const std::map<char*, project_template> templates {
  {"c", c_project}
};

#endif