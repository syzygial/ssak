#ifndef SSAK_CONFIG_HPP
#define SSAK_CONFIG_HPP

#include <filesystem>
#include <fstream>
#include <string>
#include <string_view>
#include <map>
#include <regex>

namespace fs = std::filesystem;

namespace ssak {
  class Config {
    public:
    //Config() {parse_config_file();}
    Config() : Config(std::string(std::getenv("HOME")) + "/.ssak.conf") {} // ~/.ssak.conf
    Config(const std::string& s) : config_file(s) {
      this->apply_defaults();
      if (fs::exists(this->config_file)) parse_config_file();
    }
    std::string& operator[] (const std::string& index) {return (this->config_attrs)[index];}
    static Config& Global() {
      static std::unique_ptr<Config> global_config(new Config);
      Config& c = *(global_config.get());
      return c;
    }
    private:
    void parse_config_file() {
      //unsigned int fsize = fs::file_size(config_file);
      std::ifstream config_stream(this->config_file, std::ios::in);
      static const std::regex empty_re(R"(^[ ]*$)");
      static const std::regex section_re(R"(^\s*\[\s*((?:\w+)(?:\.\w+)*)\s*\]$)");
      static const std::regex val_re(R"(^\s*(\w+)\s*=\s*(\w+)\s*$)");
      std::smatch m;
      std::string line;
      std::string current_section;
      while (std::getline(config_stream, line)) {
        if (std::regex_search(line, m, empty_re)) continue;
        else if (std::regex_search(line, m, val_re)) {
          if (current_section.empty()) this->config_attrs[m[1]] = m[2];
          else this->config_attrs[current_section + "." + (std::string)m[1]] = m[2];
        }
        else if (std::regex_search(line, m, section_re)) current_section = m[1];
        else continue;
      }
    }
    void apply_defaults() {
      (*this)["exp_root"] = std::string(std::getenv("HOME")) + "/exp";
    }
    std::string config_file;
    std::map<std::string,std::string> config_attrs;
  };
}

#endif