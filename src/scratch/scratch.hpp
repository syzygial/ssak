#include "../sqlite.hpp"

namespace ssak {
namespace scratch {

namespace {
class scratch_sqlite3_ctx : private util::sqlite3_ctx {
  public:
    scratch_sqlite3_ctx(const char *db_name);
    void add_archive(const char *name, void *archive_data, int n_bytes);
};

scratch_sqlite3_ctx::scratch_sqlite3_ctx(const char *db_name) {
  this->connect(db_name);
  const char *scratch_table_stmt =
    "CREATE TABLE IF NOT EXISTS scratch_experiments("
    "id INTEGER PRIMARY KEY ASC, "
    "exp_name TEXT, "
    "exp_archive BLOB"
    ");";
  sqlite3_stmt *stmt;
  sqlite3_prepare(this->db, scratch_table_stmt, std::strlen(scratch_table_stmt), &stmt, NULL);
  sqlite3_step(stmt);
  sqlite3_finalize(stmt);
}

void scratch_sqlite3_ctx::add_archive(const char *name, void *archive_data, int n_bytes) {
  sqlite3_stmt *stmt;
  const char *stmt_text = "INSERT INTO scratch_archive VALUES(NULL, ?1, ?2)";
  sqlite3_prepare(this->db, stmt_text, std::strlen(stmt_text), &stmt, NULL);
  sqlite3_bind_text(stmt, 1, name, std::strlen(name), SQLITE_STATIC);
  sqlite3_bind_blob(stmt, 2, archive_data, n_bytes, SQLITE_STATIC);
  sqlite3_step(stmt);
  sqlite3_finalize(stmt);
}
}

class scratch {
  public:
    scratch() : sqlite3_conn("test.db") {}
    void add_exp(const char *name);
    void del_exp(const char *name);
    void archive_exp(const char *name);
    void extract_exp(const char *name);
    void list_exp();
  private:
    scratch_sqlite3_ctx sqlite3_conn;
};

}
}