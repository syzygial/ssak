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

namespace ssak
{
  namespace task
  {

    class task
    {
    public:
      task() : sqlite3_conn(std::string(std::getenv("HOME")) + "/.ssak.db")
      { // ~/.ssak.db
        this->tasks = sqlite3_conn.get_info();
      }
      void add_task(std::string &name)
      {
      }
      task(const char *db_name) : sqlite3_conn(db_name), tasks(sqlite3_conn.get_info()) {}
      void task_graph(std::ostream& os) {
        os << "digraph G {\n";
        for (auto& [k,v] : this->tasks) {
          os << "\"" << k << "\";\n";
          for (auto& d : v.dependencies) {
            os << "\"" << k << "\"" << "->" << "\"" << d.get().name << "\";\n";
          }
        }
        os << "}" << std::endl;
      }
      void task_graph() {
        task_graph(std::cout);
      }
      void list_tasks(std::ostream &os)
      {
        for (auto &[k, v] : tasks)
        {
          os << k << " (" << v.id << ")";
          if (!v.dependencies.empty()) {
            os << " [";
            for (auto& d : v.dependencies) {
              os << d.get().name;
              if (&(d.get()) != &(v.dependencies.back().get())) {
                os << ", ";
              }
            }
            os << "]";
          }
          os << ": " << v.description << std::endl;
        }
      }
      void list_tasks()
      {
        list_tasks(std::cout);
      }

    private:
      struct task_info
      {
        int id;
        std::string name;
        std::string description;
        std::vector<std::reference_wrapper<struct task_info>> dependencies;
      };
      class task_sqlite3_ctx : private util::sqlite3_ctx
      {
      public:
        task_sqlite3_ctx(const std::string &db_name)
        {
          this->connect(db_name.c_str());
          const char *task_table_stmt =
              "CREATE TABLE IF NOT EXISTS tasks( "
              "id INTEGER PRIMARY KEY ASC, "
              "task_name TEXT UNIQUE, "
              "task_desc TEXT, "
              "task_depends TEXT" // csv of task prerequiside IDs
              ");";
          sqlite3_stmt *stmt = NULL;
          sqlite3_prepare(this->db, task_table_stmt, std::strlen(task_table_stmt), &stmt, NULL);
          sqlite3_step(stmt);
          sqlite3_finalize(stmt);
        }
        std::map<std::string, task_info> get_info()
        {
          std::map<std::string, task_info> _task_info;
          // std::vector<std::reference_wrapper<task_info> > task_info_vec;
          std::map<unsigned int, task_info*> task_info_vec;
          std::vector<unsigned int> task_id_vec;
          std::vector<std::vector<unsigned int>> deps;
          sqlite3_stmt *stmt = NULL;
          const char *stmt_text = "SELECT id,task_name,task_desc,task_depends FROM tasks";
          sqlite3_prepare(this->db, stmt_text, std::strlen(stmt_text), &stmt, NULL);
          while (sqlite3_step(stmt) != SQLITE_DONE)
          {
            struct task_info t;
            unsigned int task_id = (unsigned int)sqlite3_column_int(stmt, 0);
            task_id_vec.push_back(task_id);
            const unsigned char *task_name = sqlite3_column_text(stmt, 1);
            const unsigned char *task_desc = sqlite3_column_text(stmt, 2);
            const unsigned char *task_depends = sqlite3_column_text(stmt, 3);
            char *_task_depends = NULL;
            t.id = task_id;
            if (task_depends)
            {
              _task_depends = (char *)calloc(std::strlen((const char *)task_depends) + 1, sizeof(char));
              strcpy(_task_depends, (const char *)task_depends);
            }
            else
            {
              _task_depends = NULL;
            }
            t.name = std::string((const char *)task_name);
            if (task_desc)
            {
              t.description = std::string((const char *)task_desc);
            }
            else
            {
              t.description = std::string("n/a");
            }
            if (task_depends)
            {
              deps.push_back(csv2vec(_task_depends));
              free(_task_depends);
            }
            else
            {
              deps.push_back({});
            }
            _task_info[(const char *)task_name] = t;
            task_info_vec[task_id] = &(_task_info[(const char *)task_name]);
          }
          int task_i = 0;
          for (auto &d : deps)
          {
            std::string &task_name = (*(task_info_vec[task_id_vec[task_i]])).name;
            //std::vector<std::reference_wrapper<task_info>> task_deps;
            for (auto i : d) {
              //task_deps.push_back(*(task_info_vec[i]));
              auto& dep = *(task_info_vec[i]);
              _task_info[task_name].dependencies.push_back(dep);
            }
            task_i++;
          }
          sqlite3_finalize(stmt);
          return _task_info;
        }

      private:
        std::vector<unsigned int> csv2vec(char *s)
        {
          if (s == NULL || (*s == '\0'))
            return std::vector<unsigned int>{};
          std::vector<unsigned int> v;
          char *int_str = std::strtok(s, ",");
          while (int_str)
          {
            if ((strcmp(int_str, "0") != 0) &&
                (std::atoi(int_str) == 0))
            {
              throw std::runtime_error(std::format("Value {} cannot be converted to int", int_str));
            }
            v.push_back((unsigned int)std::atoi(int_str));
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