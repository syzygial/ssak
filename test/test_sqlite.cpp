#include "sqlite.hpp"

#include "test_main.hpp"

class sqlite_test : public ::ssak::util::sqlite3_ctx {
  public:
  sqlite_test(const char *db_name) : ::ssak::util::sqlite3_ctx(db_name) {
    const char *create_table_stmt =
      "CREATE TABLE IF NOT EXISTS test("
      "id INTEGER PRIMARY KEY ASC, "
      "test_field TEXT);";
    sqlite3_stmt *stmt = NULL;
    sqlite3_prepare(this->db, create_table_stmt, std::strlen(create_table_stmt), &stmt, NULL);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
  }
  ~sqlite_test() {}
};

int test_sqlite_conn() {
  sqlite_test test_conn("test.db");
  return 0;
}