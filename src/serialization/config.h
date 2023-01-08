#pragma once

#include <string>
#include <filesystem>

#include "logger/logger.h"

namespace hpaslt {

class Config {
 private:
  // Base directories for all config files.
  static const std::string m_baseDir;

  void checkAndCreateDir(const std::filesystem::path& fullPath);

 protected:
  /**
   * @brief The save path of current config file.
   *
   */
  std::string m_savePath;

 public:
  /**
   * @brief Construct a new Config object
   *
   * @param fileName the name of the config file.
   */
  Config(const std::string& fileName);

  /**
   * @brief Method to save the current config to the m_savePath;
   *
   */
  virtual void save() {}

  /**
   * @brief Method to load the current config from the m_savePath;
   *
   */
  virtual void load() {}
};

}  // namespace hpaslt
