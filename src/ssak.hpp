#ifndef _SSAK_HPP
#define _SSAK_HPP

#include <string>

class ssak_cmd {
public:
  typedef int (*cmd_fn) (int argc, char **argv);
  ssak_cmd(cmd_fn cmd) : cmd(cmd) {}
  int exec(int argc, char **argv) { return this->cmd(argc, argv); }
private:
  cmd_fn cmd;
};

class ssak_tool {
public:
  ssak_tool(std::string name) : name(name) {}
private:
  std::string name;
};

#endif