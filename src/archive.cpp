#include <archive.h>
#include <archive_entry.h>

#include <algorithm>
#include <filesystem>
#include <vector>

#include <sys/stat.h>

#include "archive.hpp"

namespace fs = std::filesystem;

namespace ssak {
  namespace {
    class fs_name_itr {
      public:
        typedef int difference_type;
        typedef fs::directory_entry value_type;
        typedef fs::directory_entry* pointer;
        typedef fs::directory_entry& reference;
        typedef std::forward_iterator_tag iterator_category;

        fs_name_itr(std::vector<fs::directory_entry>& v) : v(v) {}
        fs_name_itr begin() {
          fs_name_itr itr(this->v);
          itr.i = 0;
          return itr;
        }
        fs_name_itr end() {
          fs_name_itr itr(this->v);
          itr.i = v.size();
          return itr;
        }
        const char* operator*() const {
          return v[i].path().c_str();
        }
        fs_name_itr& operator++() {
          i++;
          return *this;
        }
        fs_name_itr operator++(int) {
          fs_name_itr prev = *this;
          ++(*this);
          return prev;
        }
        bool operator==(const fs_name_itr& rhs) const {
          return ((this->i == rhs.i) && (this->v == rhs.v));
        }
        bool operator!=(const fs_name_itr& rhs) const {
          return !(*this == rhs);
        }
      private:
        std::vector<fs::directory_entry>& v;
        int i = 0;
    };
    std::vector<fs::directory_entry> get_file_list(const char *dirname) {
      // find all files in directory
      auto files_itr = fs::recursive_directory_iterator(dirname);
      std::vector<fs::directory_entry> ar_files(fs::begin(files_itr), fs::end(files_itr));
      auto files_end = std::remove_if(ar_files.begin(), ar_files.end(), [](fs::directory_entry& f){return !(f.is_regular_file());});
      ar_files.erase(files_end, ar_files.end());
      return ar_files;
    }
  }
  void create_archive(const char *dirname, const char *archive_name) {
    struct archive *a = archive_write_new();
    archive_write_add_filter_gzip(a);
    archive_write_set_format_pax_restricted(a);
    archive_write_open_filename(a, archive_name);
    struct archive_entry *e = NULL;
    auto ar_files = get_file_list(dirname);
    for (auto f : ar_files) {
      int sz = fs::file_size(f.path());
      e = archive_entry_new();
      archive_entry_set_pathname(e, f.path().c_str());
      archive_entry_set_size(e, sz);
      archive_entry_set_filetype(e, AE_IFREG);
      archive_entry_set_perm(e, 0644);
      archive_write_header(a,e);
      std::FILE* fp = fopen(f.path().c_str(), "r");
      char *buf = (char*)malloc(sz);
      fread(buf, 1, sz, fp);
      archive_read_data(a, buf, sz);
      free(buf);
      fclose(fp);
      archive_entry_free(e);
    }
    archive_write_close(a);
    archive_write_free(a);
  }
}