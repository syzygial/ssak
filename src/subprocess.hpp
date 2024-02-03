#ifndef _SUBPROCESS_HPP_
#define _SUBPROCESS_HPP_

#include <cstdlib>
#include <unistd.h>
#include <sys/wait.h>

namespace ssak {
class subprocess {
  public:
    int run();
  private:
    virtual int pre_hook() = 0;
    virtual int post_hook() = 0;
};

int subprocess::run() {
  pre_hook();
  pid_t pid = fork();
  if (pid > 0) {
    int cmd_status = 0;
    waitpid(pid,&cmd_status,0);
    return 0;
  }
  else if (pid == 0) {

  }
  else { // pid < 0
    return -1;
  }
  return 0;
}

} // namespace ssak


#endif