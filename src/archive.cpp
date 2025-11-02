
#include <algorithm>
#include <array>
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
  static void _create_archive(struct archive *a, const char *dirname) {
    fs::path exp_parent = fs::path(dirname).parent_path();
    struct archive_entry *e = NULL;
    auto ar_files = get_file_list(dirname);
    for (auto f : ar_files) {
      int sz = fs::file_size(f.path());
      e = archive_entry_new();
      //archive_entry_set_pathname(e, f.path().c_str());
      archive_entry_set_pathname(e, fs::relative(f.path(), exp_parent).c_str());
      archive_entry_set_size(e, sz);
      archive_entry_set_filetype(e, AE_IFREG);
      #ifdef __unix__
      struct stat s;
      stat(f.path().c_str(), &s);
      archive_entry_set_perm(e, s.st_mode);
      #else
      archive_entry_set_perm(e, 0644);
      #endif
      archive_write_header(a,e);
      std::FILE* fp = fopen(f.path().c_str(), "r");
      char *buf = (char*)malloc(sz);
      fread(buf, 1, sz, fp);
      archive_write_data(a, buf, sz);
      free(buf);
      fclose(fp);
      archive_entry_free(e);
    }    
  }
  void create_archive(const char *dirname, const char *archive_name) {
    struct archive *a = archive_write_new();
    // TODO: parse filename to figure out which compression filters to use
    archive_write_add_filter_gzip(a);
    archive_write_set_format_pax_restricted(a);
    archive_write_open_filename(a, archive_name);
    _create_archive(a, dirname);
    archive_write_close(a);
    archive_write_free(a);
  }
void* create_archive(const char *dirname, size_t *archive_sz) {
  auto files_itr = fs::recursive_directory_iterator(dirname);
  size_t buf_sz = 0;
  *archive_sz = 0;
  // guesstimate the size of the buffer we need to allocate
  for (auto f: files_itr) {
    if (fs::is_regular_file(f.path())) {
      buf_sz += fs::file_size(f.path());
    }
  }
  buf_sz += 0x2000;
  void *archive_buf = malloc(buf_sz);
  struct archive *a = archive_write_new();
  archive_write_add_filter_gzip(a);
  archive_write_set_format_pax_restricted(a);
  archive_write_open_memory(a, archive_buf, buf_sz, archive_sz);
  //archive_write_set_bytes_per_block(a, 0);
  _create_archive(a, dirname);
  archive_write_close(a);
  archive_write_free(a);
  return archive_buf;
}

void extract_archive(void *archive, size_t archive_len, const std::string& root_dir) {
  archive_itr itr(archive, archive_len);
  for (const auto &[fname, file_mode, file_contents] : itr) {
    fs::path fname_path(root_dir + "/" + fname);
    if (!fs::exists(fname_path.parent_path())) {
      fs::create_directories(fname_path.parent_path());
    }
    FILE *f = fopen(fname_path.c_str(), "wb");
    fwrite(file_contents.c_str(), 1, file_contents.length(), f);
    #ifdef __unix__
    int fd = fileno(f);
    fchmod(fd, file_mode);
    #endif
    fclose(f);
  }
}

} // namespace ssak