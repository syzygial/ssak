#ifndef SSAK_ARCHIVE_HPP
#define SSAK_ARCHIVE_HPP

#include <cstddef>
#include <filesystem>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

#include <archive.h>
#include <archive_entry.h>

namespace fs = std::filesystem;

namespace ssak {
  void create_archive(const char *dirname, const char *archive_name);
  void* create_archive(const char *dirname, size_t *archive_sz); // in memory
  void extract_archive(void *archive, size_t archive_len, const std::string root_dir);

  class archive_itr {
    public:
    typedef int difference_type;
    //typedef archive_entry* value_type;
    typedef std::pair<std::string, std::string> value_type;
    typedef value_type* pointer;
    typedef value_type& reference;
    typedef std::forward_iterator_tag iterator_category;

    archive_itr(const char *fname) {
      this->archive_managed = true;
      this->a = archive_read_new();
      archive_read_support_format_tar(this->a);
      archive_read_support_filter_gzip(this->a);
      if (archive_read_open_filename(this->a, fname, 0x2000) != ARCHIVE_OK) {
        throw std::runtime_error("couldn't read archive");
      }
      if (archive_read_next_header(this->a, &(this->e)) != ARCHIVE_OK) {
        this->i = -1;
      }
    }
    archive_itr(struct archive *a) : a(a) {}
    archive_itr(void *archive_data, size_t len) {
      this->archive_managed = true;
      this->a = archive_read_new();
      archive_read_support_format_tar(this->a);
      archive_read_support_filter_gzip(this->a);
      if (archive_read_open_memory(this->a, archive_data, len) != ARCHIVE_OK) {
        throw std::runtime_error("couldn't read archive");
      }
      if (archive_read_next_header(this->a, &(this->e)) != ARCHIVE_OK) {
        this->i = -1;
      }
    }
    archive_itr() = default;
    ~archive_itr() {
      if (this->archive_managed) {
        archive_read_free(this->a);
      }
    }
    archive_itr begin() {
      archive_itr itr(this->a);
      itr.e = this->e;
      itr.i = 0;
      return itr;
    }
    static archive_itr& end() {
      static std::unique_ptr<archive_itr> itr(new archive_itr);
      itr->a = NULL;
      itr->e = NULL;
      itr->i = -1;
      return *itr;
    }
    value_type operator*() const {
      //return this->e;
      int64_t file_size = archive_entry_size(this->e);
      void *raw_dat = malloc(file_size);
      int bytes_read = archive_read_data(a, raw_dat, file_size);
      std::string fname(archive_entry_pathname(this->e));
      std::string byte_str;
      if (bytes_read > 0) {
        byte_str.append((char*)raw_dat, bytes_read);
      }
      free(raw_dat);
      return {fname, byte_str};
    }
    bool operator==(const archive_itr& rhs) const {
      if (rhs.a != NULL) {
        return (this->i == rhs.i) && (this->a == rhs.a);
      }
      else {
        return this->i == rhs.i;
      }
    }
    bool operator!=(const archive_itr& rhs) const {
      return !((*this) == rhs);
    }
    archive_itr& operator++() {
      if (archive_read_next_header(this->a, &(this->e)) != ARCHIVE_OK) {
        this->i = -1;
      }
      else {
        this->i++;
      }
      return *this;
    }
    archive_itr operator++(int) {
      archive_itr prev = *this;
      (*this)++;
      return prev;
    }
    private:
    struct archive *a = NULL;
    struct archive_entry *e = NULL;
    bool archive_managed = false;
    int i = 0;
  };

} // namespace ssak

#endif