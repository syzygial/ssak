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
  if (fs::exists(dir) && !fs::is_directory(dir)) throw fs::filesystem_error("dir must be directory", std::error_code());
  static fs::path sys_tmp_dir = fs::temp_directory_path();
  if (dir.is_absolute()) {
    this->root_dir = dir;
  }
  else {
    this->root_dir = sys_tmp_dir / dir;
  }
  if (!fs::exists(this->root_dir)) fs::create_directories(this->root_dir);
  this->current_file = fs::path();
}

~tmpdir() {
  fs::remove_all(this->root_dir);
}

// change current file
tmpdir& operator<<(fs::path rhs) {
  if (fs::exists(rhs) && !fs::is_regular_file(rhs)) throw std::runtime_error("tmpdir << rhs must be a file");
  if (rhs.is_absolute() && !((std::string)(rhs.parent_path()))
      .starts_with((std::string)(this->root_dir)))
    throw std::runtime_error("rhs must be inside of root_dir");
  this->current_file = (rhs.is_absolute()) ? rhs : this->root_dir / rhs;
  if (!(this->current_file).parent_path().empty() && 
      !fs::exists((this->current_file).parent_path())) 
    fs::create_directories((this->current_file).parent_path());
  return *this;
}

// write to current file
tmpdir& operator<<(const char *rhs) {
  if ((this->current_file).empty()) throw std::runtime_error("current_file not set");
  std::ofstream o(this->current_file, std::ios::app);
  o << rhs;
  o.flush();
  return *this;
}

private:
fs::path root_dir;
fs::path current_file;
};

} // namespace util
} // namespace ssak

#endif