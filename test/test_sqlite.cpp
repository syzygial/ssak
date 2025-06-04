#include <filesystem>
#include <memory>
#include <string>
#include <sstream>
#include <tuple>

#include "sqlite.hpp"

#include "test_main.hpp"

template<typename T>
std::tuple<T> get_sqlite_col(sqlite3_stmt *stmt, int iCol);

template<typename T, typename... Ts> requires (sizeof...(Ts) > 0)
std::tuple<T,Ts...> get_sqlite_col(sqlite3_stmt *stmt, int iCol = 0) {
  std::tuple<T> column = get_sqlite_col<T>(stmt, iCol);
  return std::tuple_cat(column, get_sqlite_col<Ts...>(stmt, iCol+1));
}

template<>
std::tuple<int> get_sqlite_col(sqlite3_stmt *stmt, int iCol) {
  return std::make_tuple(sqlite3_column_int(stmt, iCol));
}

template<>
std::tuple<int64_t> get_sqlite_col(sqlite3_stmt *stmt, int iCol) {
  return std::make_tuple(sqlite3_column_int64(stmt, iCol));
}

template<>
std::tuple<const unsigned char*> get_sqlite_col(sqlite3_stmt *stmt, int iCol) {
  return std::make_tuple(sqlite3_column_text(stmt, iCol));
}

template<typename... Args>
class Sqlite3Itr {
  public:
  using difference_type = int;
  using value_type = std::tuple<Args...>;
  Sqlite3Itr(sqlite3 *db, const char *table_name) : db(db) {
    std::ostringstream s;
    s << "SELECT * FROM " << table_name << ";";
    sqlite3_prepare(this->db, s.str().c_str(), std::strlen(s.str().c_str()), &(this->stmt), NULL);
    sqlite3_step(this->stmt);
  }
  //Sqlite3Itr (::ssak::util::sqlite3_ctx &ctx) : db(ctx.db) {}
  ~Sqlite3Itr() {
    sqlite3_finalize(this->stmt);
  }
  bool operator==(const Sqlite3Itr<Args...> &rhs) const {
    return false;
  }

  value_type operator*() const {
    return get_sqlite_col<Args...>(this->stmt);
  }

  static Sqlite3Itr<Args...> &end() {
    static std::unique_ptr<Sqlite3Itr<Args...> > itr(new Sqlite3Itr<Args...>);
    itr->i = -1;
    return *itr;
  }

  private:
  template<typename T>
  std::tuple<T> gen_tuple(T v) {return std::make_tuple(v);}
  template<typename T, typename... Ts>
  std::tuple<T, Ts...> gen_tuple(T t,Ts... args) {
    auto T_tuple = gen_tuple<T>(t);
    return std::tuple_cat(T_tuple, gen_tuple<Ts...>(args...));
  }

  sqlite3 *db = nullptr;
  sqlite3_stmt *stmt = nullptr;
  int i = 0;
  value_type current_value;
};

class sqlite_test : public ::ssak::util::sqlite3_ctx {
  public:
  using iterator = Sqlite3Itr<int, const unsigned char*>;

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

  iterator itr() {
    return iterator(this->db, "test");
  }
};

namespace fs = std::filesystem;

int test_sqlite_conn() {
  fs::remove("test.db");
  sqlite_test test_conn("test.db");
  test_conn.add_data();
  auto itr = test_conn.itr();
  auto [id, test_field] = *itr;
  assert(id == 1);
  assert(std::string((const char*)test_field) == std::string("hello world"));
  return 0;
}