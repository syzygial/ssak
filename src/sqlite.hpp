#include <stddef.h> // NULL
#include <sqlite3.h>
#include <cstring>
namespace ssak {
namespace util {

class sqlite3_ctx {
public:
  sqlite3_ctx() {}
  sqlite3_ctx(const char *db_name);
  int connect(const char *db_name);
  int use_db(const char *db_name);
  operator bool() const { return (this->db != NULL); }
private:
  sqlite3 *db = NULL;
};

int sqlite3_ctx::connect(const char *db_name) {
  if (*this) sqlite3_close(this->db);
  sqlite3_open(db_name, &(this->db));
}

sqlite3_ctx::sqlite3_ctx(const char *db_name) {
  this->connect(db_name);
}

sqlite3_ctx::~sqlite3_ctx() {
  sqlite3_close(this->db);
}

int sqlite3_ctx::use_db(const char *db) {
  const char *stmt_txt = "USE ?1";
  sqlite3_stmt *stmt;
  sqlite3_prepare(this->db, stmt_txt, std::strlen(stmt_txt), &stmt, NULL);
  sqlite3_bind_text(stmt, 1, db, std::strlen(db), SQLITE_STATIC);
  sqlite3_step(stmt);
  sqlite3_finalize(stmt);
}

}
}