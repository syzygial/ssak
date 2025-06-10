#ifndef SSAK_CONFIG_HPP
#define SSAK_CONFIG_HPP

#include <filesystem>
#include <fstream>
#include <string>
#include <map>
#include <regex>

namespace fs = std::filesystem;

namespace ssak {
  class Config {
    public:
    Config() {parse_config_file();}
    Config(std::string s) : config_file(s) {parse_config_file();}
    const char* operator[] (const char* index) const {return (this->config_attrs).at(index).c_str();}
    static Config& Global() {
      static std::unique_ptr<Config> global_config(new Config);
      return *(global_config.get());
    }
    private:
    void parse_config_file() {
      //unsigned int fsize = fs::file_size(config_file);
      std::ifstream config_stream(this->config_file, std::ios::in);
      static const std::regex empty_re(R"(^[ ]*$)");
      static const std::regex section_re(R"(^[((?\w)(?\.(\w))*)]$)");
      static const std::regex val_re(R"((\w)=(\w))");
      std::smatch m;
      std::string line;
      std::string current_section;
      while (std::getline(config_stream, line)) {
        if (std::regex_search(line, m, empty_re)) continue;
        else if (std::regex_search(line, m, section_re)) this->config_attrs[m[0]] = m[1];
        else if (std::regex_search(line, m, val_re)) current_section = m[0];
        else continue;
      }
    }
    std::string config_file = "~/.ssak.conf";
    std::map<std::string,std::string> config_attrs;
  };
}

#endif