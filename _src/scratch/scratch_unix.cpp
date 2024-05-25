#include <cstdlib>
#include <string>

#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>

#include "scratch.hpp"

namespace ssak {

int ssak_scratch_archive(std::string exp_name, util::sqlite3_ctx &db) {
  pid_t iso_cmd = fork();
  int shm_fd = shm_open("ssak_scratch_archive", O_RDWR | O_CREAT, 0600);
  if (iso_cmd < 0) {
    fprintf(stderr, ":(\n");
    exit(1);
  }
  else if (iso_cmd == 0)  { // child
    
  }
  else { // parent
    waitpid(iso_cmd, NULL, 0);
  }
  return 0;
}

}