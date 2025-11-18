#ifndef SSAK_TASK_HPP
#define SSAK_TASK_HPP

#include <cstring>
#include <format>
#include <functional>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>

#include "../sqlite.hpp"

namespace ssak {
namespace task {

class task {
  public:
  task() : sqlite3_conn(std::string(std::getenv("HOME")) + "/.ssak.db") {} // ~/.ssak.db
  void list_tasks(std::ostream &os) {
    for (auto& [k,v] : tasks) {
      os << k << ": " << v.description << std::endl;
    }
  }
  void list_tasks() {
    list_tasks(std::cout);
  }
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
      std::vector<std::reference_wrapper<task_info> > task_info_vec;
      std::vector<std::vector<int> > deps;
      sqlite3_stmt *stmt = NULL;
      const char *stmt_text = "SELECT id,task_name,task_desc,task_depends FROM tasks";
      sqlite3_prepare(this->db, stmt_text, std::strlen(stmt_text), &stmt, NULL);
      while (sqlite3_step(stmt) != SQLITE_DONE) {
        struct task_info t;
        int task_id = sqlite3_column_int(stmt, 0);
        const unsigned char *task_name = sqlite3_column_text(stmt, 1);
        const unsigned char *task_desc = sqlite3_column_text(stmt, 2);
        const unsigned char *task_depends = sqlite3_column_text(stmt, 3);
        char *_task_depends = (char*)calloc(std::strlen((const char*)task_depends)+1,sizeof(char));
        strcpy(_task_depends,(const char*)task_depends);
        t.name = std::string((const char*)task_name);
        t.description = std::string((const char*)task_desc);
        deps.push_back(csv2vec(_task_depends));
        free(_task_depends);
        _task_info[(const char*)task_name] = t;
        task_info_vec.push_back(t);
      }
      for (auto& d : deps) {
        int task_i = 0;
        std::vector<std::reference_wrapper<task_info> > task_deps;
        for (auto i : d) {
          task_deps.push_back(task_info_vec[i]);
        }
        std::string& task_name = task_info_vec[task_i].get().name;
        _task_info[task_name].dependencies = task_deps;
        task_i++;
      }
      sqlite3_finalize(stmt);
      return _task_info;
    }

    private:
    std::vector<int> csv2vec(char* s) {
      if (*s == '\0') return std::vector<int>{};
      std::vector<int> v;
      char *int_str = std::strtok(s, ",");
      while (int_str) {
        if ((strcmp(int_str, "0") != 0) &&
          (std::atoi(int_str) == 0)) {
            throw std::runtime_error(std::format("Value {} cannot be converted to int", int_str));
          }
        v.push_back(std::atoi(int_str));
        int_str = std::strtok(NULL, ",");
      }
      return v;
    }
  };
  task_sqlite3_ctx sqlite3_conn;
  std::map<std::string, task_info> tasks;
};

} // namespace task
} // namespace ssak

#endif