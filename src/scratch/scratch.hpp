#ifndef SSAK_SCRATCH_HPP
#define SSAK_SCRATCH_HPP

#include <iostream>
#include <set>
#include <utility>

#include "../archive.hpp"
#include "../config.hpp"
#include "../sqlite.hpp"
#include "../exp_templates.hpp"

namespace fs = std::filesystem;

namespace ssak {
namespace scratch {

class scratch {
  public:
  scratch() : sqlite3_conn("test.db"), experiments(sqlite3_conn.get_info()) {}
  void create_exp(const char *name) {
    fs::path exp_path(std::string(Config::Global()["exp_root"]) + "/" + std::string(name));
    fs::create_directory(exp_path);
    this->create_exp(name, exp_path);
  }
  void create_exp(const char *name, fs::path& exp_path) {
    if (name_taken(name)) return;
    fs::create_directories(exp_path);
  }
  void del_exp(const char *name) {
    auto &_exp_info = experiments.at(name);
    if (!_exp_info.is_archived) {
      fs::remove_all(_exp_info.exp_root);
    }
    sqlite3_conn.delete_archive(name);
  }
  void archive_exp(const char *name) {
    auto& archive_exp_info = experiments.at(name);
    if (archive_exp_info.is_archived) return;
    size_t blob_size = 0;
    void *exp_blob = create_archive(archive_exp_info.exp_root.c_str(), &blob_size);
    sqlite3_conn.add_archive(name, exp_blob, true, blob_size);
    fs::remove_all(archive_exp_info.exp_root);
    archive_exp_info.is_archived = true;
  }
  void extract_exp(const char *name, fs::path& dir) {
    auto& _exp_info = experiments.at(name);
    if (!_exp_info.is_archived) return;
    const auto& [archive_dat, archive_len] = sqlite3_conn.extract_archive(name);
    extract_archive(archive_dat, archive_len, dir);
  }
  void list_exp(std::ostream &os) {
    for (auto& [k,v] : experiments) {
      os << k << std::endl;
    }
  }
  void list_exp() {
    list_exp(std::cout);
  }
  void apply_template(const char *exp_name, const char *template_name) {
    const fs::path& exp_root = this->get_exp_path(exp_name);
    initialize_template(template_name, exp_root);
  }

  private:
  struct exp_info {
    fs::path exp_root;
    bool is_archived;
  };
  class scratch_sqlite3_ctx : private util::sqlite3_ctx {
    public:
    scratch_sqlite3_ctx(const char *db_name) {
      this->connect(db_name);
      const char *scratch_table_stmt =
        "CREATE TABLE IF NOT EXISTS scratch_experiments("
        "id INTEGER PRIMARY KEY ASC, "
        "exp_name TEXT, "
        "exp_archived BOOLEAN, "
        "exp_data BLOB" // contains .tar.gz if archived, pathname otherwise
        ");";
      sqlite3_stmt *stmt = NULL;
      sqlite3_prepare(this->db, scratch_table_stmt, std::strlen(scratch_table_stmt), &stmt, NULL);
      sqlite3_step(stmt);
      sqlite3_finalize(stmt);
    }
    void add_archive(const char *name, void *archive_data, bool exp_archived, int n_bytes) {
      sqlite3_stmt *stmt;
      const char *exp_archived_str = (exp_archived) ? "TRUE" : "FALSE";
      const char *stmt_text = "INSERT INTO scratch_archive VALUES(NULL, ?1, ?2, ?3)";
      sqlite3_prepare(this->db, stmt_text, std::strlen(stmt_text), &stmt, NULL);
      sqlite3_bind_text(stmt, 1, name, std::strlen(name), SQLITE_STATIC);
      sqlite3_bind_text(stmt, 2, exp_archived_str, std::strlen(exp_archived_str), SQLITE_STATIC);
      sqlite3_bind_blob(stmt, 3, archive_data, n_bytes, SQLITE_STATIC);
      sqlite3_step(stmt);
      sqlite3_finalize(stmt);
    }
    std::pair<void*,size_t> extract_archive(const char *name) {
      sqlite3_stmt *stmt;
      const char *stmt_text = "SELECT exp_data FROM scratch_archive WHERE name=$1";
      sqlite3_prepare(this->db, stmt_text, std::strlen(stmt_text), &stmt, NULL);
      sqlite3_bind_text(stmt, 1, name, std::strlen(name), SQLITE_STATIC);
      sqlite3_step(stmt);
      const void *exp_data = sqlite3_column_blob(stmt, 0);
      size_t exp_data_len = sqlite3_column_bytes(stmt, 0);
      void *_exp_data = memcpy(_exp_data, exp_data, exp_data_len);
      sqlite3_finalize(stmt);
      return {_exp_data, exp_data_len};
    }
    void delete_archive(const char *name) {
      sqlite3_stmt *stmt;
      const char *stmt_text = "DELETE FROM scratch_archive WHERE exp_name=?1";
      sqlite3_prepare(this->db, stmt_text, std::strlen(stmt_text), &stmt, NULL);
      sqlite3_bind_text(stmt, 1, name, std::strlen(name), SQLITE_STATIC);
      sqlite3_step(stmt);
      sqlite3_finalize(stmt);
    }
    std::map<const char*, exp_info> get_info() {
      sqlite3_stmt *stmt;
      std::map<const char*, exp_info> _exp_info;
      const char *stmt_text = "SELECT exp_name,is_archived,exp_data FROM scratch_archive";
      sqlite3_prepare(this->db, stmt_text, std::strlen(stmt_text), &stmt, NULL);
      while (sqlite3_step(stmt) != SQLITE_DONE) {
        const unsigned char *exp_name = sqlite3_column_text(stmt, 0);
        size_t exp_name_len = sqlite3_column_bytes(stmt, 0);
        char *_exp_name = (char*)malloc(exp_name_len);
        struct exp_info e;
        std::memcpy(_exp_name, exp_name, exp_name_len);
        int exp_archived = sqlite3_column_int(stmt, 1);
        if (exp_archived) {
          e.is_archived = true;
          e.exp_root = fs::path();
        }
        else {
          e.is_archived = false;
          const unsigned char *exp_path = sqlite3_column_text(stmt, 2);
          size_t exp_path_len = sqlite3_column_bytes(stmt, 2);
          char *_exp_path = (char*)malloc(exp_path_len);
          std::memcpy(_exp_path, exp_path, exp_path_len);
          e.exp_root = fs::path(_exp_path);
        }
        _exp_info[_exp_name] = e;
      }
      sqlite3_finalize(stmt);
      return _exp_info;
    }
  };

  bool name_taken(const char *name) {
    return (experiments.count(name) > 0);
  }
  const fs::path& get_exp_path(const char *name) {
    if (!name_taken(name)) return fs::path();
    return experiments.at(name).exp_root;
  }
  scratch_sqlite3_ctx sqlite3_conn;
  std::map<const char*, exp_info> experiments;
};

}
}

#endif