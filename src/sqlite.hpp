#include <stddef.h> // NULL
#include <sqlite3.h>

namespace ssak {
namespace util {

class sqlite3_ctx {
public:
  sqlite3_ctx() {}
  sqlite3_ctx(const char *db_name);
  int connect(const char *db_name);
  operator bool() const { return (this->db != NULL); }
private:
  sqlite3 *db = NULL;
};

int sqlite3_ctx::connect(const char *db_name) {
  if (*this) sqlite3_close(this->db);
  sqlite3_open(db_name, &(this->db));
}

sqlite3_ctx::sqlite3_ctx(const char *db_name) {
  sqlite3_open(db_name, &(this->db));
}

sqlite3_ctx::~sqlite3_ctx() {
  sqlite3_close(this->db);
}

}
}