#ifndef SSAK_UTIL_TMPFILES_HPP
#define SSAK_UTIL_TMPFILES_HPP

#include <filesystem>
#include <fstream>
#include <system_error>

namespace fs = std::filesystem;

namespace ssak {
namespace util {

class tmpdir {
public:

tmpdir(fs::path dir) {
  if (!fs::is_directory(dir)) throw fs::filesystem_error("dir must be directory", std::error_code());
  static fs::path sys_tmp_dir = fs::temp_directory_path();
  if (dir.is_absolute()) {
    this->root_dir = dir;
  }
  else {
    this->root_dir = sys_tmp_dir / dir;
  }
  this->current_file = fs::path();
}

~tmpdir() {
  fs::remove_all(this->root_dir);
}

// change current file
tmpdir& operator<<(fs::path &rhs) {
  if (!fs::is_regular_file(rhs)) throw fs::filesystem_error("tmpdir << rhs must be a file", std::error_code());
  if (!((std::string)(rhs.parent_path()))
      .starts_with((std::string)(this->root_dir)))
    throw fs::filesystem_error("rhs must be inside of root_dir", std::error_code());
  if (!fs::exists(rhs.parent_path())) fs::create_directories(rhs.parent_path());
  this->current_file = rhs;
  return *this;
}

// write to current file
tmpdir& operator<<(std::string &rhs) {
  std::ofstream o(this->current_file, std::ios::app);
  o << rhs;
  return *this;
}

private:
fs::path root_dir;
fs::path current_file;
};

} // namespace util
} // namespace ssak

#endif