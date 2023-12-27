#include <sqlite3.h>

#include "ssak.hpp"

namespace ssak {
namespace util {

class sqlite3_ctx {
public:
  sqlite3_ctx(const char *db_name);
private:
  sqlite3 *db;
};

sqlite3_ctx::sqlite3_ctx(const char *db_name) {
  sqlite3_open(db_name, &(this->db));
}
sqlite3_ctx::~sqlite3_ctx() {
  sqlite3_close(this->db);
}
}
}