#ifndef _SCRATCH_HPP
#define _SCRATCH_HPP

#include "../sqlite.hpp"

namespace ssak {
namespace scratch {

class scratch_sqlite3_ctx : public util::sqlite3_ctx {
  public:
    int add_archive(void *tgz_data, unsigned int tgz_size, const char *tgz_name);
};

}
}

#endif