#pragma once

#include <string>

namespace hpaslt {

class Config {
 protected:
  /**
   * @brief The save path of current config file.
   *
   */
  std::string m_savePath;

 public:
  Config(const std::string& savePath) : m_savePath(savePath) {}

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
