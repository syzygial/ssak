#ifndef _SCRATCH_HPP
#define _SCRATCH_HPP

#include <algorithm>
#include <filesystem>
#include <sstream>
#include <vector>

#include <archive.h>
#include <archive_entry.h>

#include "../sqlite.hpp"

namespace fs = std::filesystem;

namespace ssak {
namespace scratch {

class scratch_sqlite3_ctx : public util::sqlite3_ctx {
  public:
    int create_archive(const char *dirname);
    int add_archive(void *tgz_data, unsigned int tgz_size, const char *tgz_name);
    int delete_archive(const char *tgz_name);
    int extract_archive();
};

int scratch_sqlite3_ctx::create_archive(const char *dirname) {
  struct archive *ar = archive_write_new();
  struct archive_entry *e = NULL;
  archive_write_add_filter_gzip(ar);
  archive_write_set_format_pax_restricted(ar);
  auto files_itr = fs::recursive_directory_iterator(dirname);
  std::vector<fs::directory_entry> ar_files(fs::begin(files_itr), fs::end(files_itr));
  auto files_end = std::remove_if(ar_files.begin(), ar_files.end(), [](fs::directory_entry& f){return f.is_regular_file();});
  ar_files.erase(files_end, ar_files.end());
  for (auto& f : ar_files) {

  }
  archive_write_close(ar);
  archive_write_free(ar);
  return 0;
}

int scratch_sqlite3_ctx::add_archive(void *tgz_data, unsigned int tgz_size, const char *tgz_name) {
  return 0;
}

}
}

#endif