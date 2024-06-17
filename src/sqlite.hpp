#include <stddef.h> // NULL
#include <sqlite3.h>
#include <cstring>

#include <vector>
#include <string>

namespace ssak {
namespace util {

class sqlite3_ctx {
public:
  sqlite3_ctx() {}
  sqlite3_ctx(const char *db_name);
  ~sqlite3_ctx();
  void connect(const char *db_name);
  void use_db(const char *db_name);
  operator bool() const { return (this->db != NULL); }
private:
  sqlite3 *db = NULL;
  std::vector<std::string> tables;
  void get_tables();
};

void sqlite3_ctx::connect(const char *db_name) {
  if (*this) sqlite3_close(this->db);
  sqlite3_open(db_name, &(this->db));
}

sqlite3_ctx::sqlite3_ctx(const char *db_name) {
  this->connect(db_name);
  this->get_tables();
}

sqlite3_ctx::~sqlite3_ctx() {
  sqlite3_close(this->db);
}

void sqlite3_ctx::use_db(const char *db) {
  const char *stmt_txt = "USE ?1";
  sqlite3_stmt *stmt;
  sqlite3_prepare(this->db, stmt_txt, std::strlen(stmt_txt), &stmt, NULL);
  sqlite3_bind_text(stmt, 1, db, std::strlen(db), SQLITE_STATIC);
  sqlite3_step(stmt);
  sqlite3_finalize(stmt);
}

void sqlite3_ctx::get_tables() {
  const char *stmt_txt = "SELECT NAME FROM sqlite_schema WHERE TYPE='table'";
  sqlite3_stmt *stmt;
  sqlite3_prepare(this->db, stmt_txt, std::strlen(stmt_txt), &stmt, NULL);
  while (sqlite3_step(stmt) == SQLITE_ROW) {
    this->tables.push_back((const char*)(sqlite3_column_text(stmt, 0)));
  }
  sqlite3_finalize(stmt);
}

}
}