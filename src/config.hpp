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
      static const std::string empty_re = R"(^[ ]*$)";
      static const std::string section_re = R"(^[(\w)\.(\w)]$)";
      static const std::string val_re = R"((\w)=(\w))";
      std::string line;
      while (std::getline(config_stream, line)) {

      }
    }
    std::string config_file = "~/.ssak.conf";
    std::map<std::string,std::string> config_attrs;
  };
}

#endif