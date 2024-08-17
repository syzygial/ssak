#ifndef SSAK_CONFIG_HPP
#define SSAK_CONFIG_HPP

#include <filesystem>
#include <string>
#include <map>

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
      unsigned int fsize = fs::file_size(config_file);
    }
    std::string config_file = "~/.ssak/config";
    std::map<std::string,std::string> config_attrs;
  };
}

#endif