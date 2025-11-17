#ifndef SSAK_TASK_HPP
#define SSAK_TASK_HPP

#include <functional>
#include <map>
#include <string>

#include "../sqlite.hpp"

namespace ssak {
namespace task {

class task {
  public:
  task() : sqlite3_conn(std::string(std::getenv("HOME")) + "/.ssak.db") {} // ~/.ssak.db
  
  private:
  struct task_info {
    std::string name;
    std::string description;
    std::vector<std::reference_wrapper<struct task_info> > dependencies;
  };
  class task_sqlite3_ctx : private util::sqlite3_ctx {
    public:
    task_sqlite3_ctx(const std::string& db_name) { 
      this->connect(db_name.c_str());
      const char *task_table_stmt = 
        "CREATE TABLE IF NOT EXISTS tasks( "
        "id INTEGAR PRIMARY KEY ASC, "
        "task_name TEXT UNIQUE, "
        "task_desc TEXT, "
        "task_depends TEXT" // csv of task prerequiside IDs
        ");";
      sqlite3_stmt *stmt = NULL;
      sqlite3_prepare(this->db, task_table_stmt, std::strlen(task_table_stmt), &stmt, NULL);
      sqlite3_step(stmt);
      sqlite3_finalize(stmt);
    }
    std::map<std::string, task_info> get_info() {
      std::map<std::string, task_info> _task_info;

      return _task_info;
    }

    private:
    std::vector<int> csv2vec(const std::string& s) {
      
    }
  };
  task_sqlite3_ctx sqlite3_conn;
  std::map<std::string, task_info> tasks;
};

} // namespace task
} // namespace ssak

#endif