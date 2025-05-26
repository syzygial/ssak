#include "sqlite.hpp"

#include "test_main.hpp"

template<typename... Args>
class Sqlite3Itr {
  public:
  Sqlite3Itr(sqlite3 *db) : db(db) {}

  private:
  sqlite3 *db = nullptr;
};

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
  int add_data() {
    sqlite3_stmt *stmt;
    const char *stmt_text = "INSERT INTO test VALUES(NULL, \"hello world\")";
    sqlite3_prepare(this->db, stmt_text, std::strlen(stmt_text), &stmt, NULL);
    sqlite3_step(stmt);
    return sqlite3_finalize(stmt);
  }
};

int test_sqlite_conn() {
  sqlite_test test_conn("test.db");
  test_conn.add_data();
  return 0;
}