#pragma once

#include <cereal/archives/json.hpp>
#include <exception>
#include <filesystem>
#include <fstream>
#include <string>

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

  /**
   * @brief Helper method to get the output archive.
   *
   * @return std::shared_ptr<cereal::JSONOutputArchive>
   */
  template <class T>
  void saveHelper(T& target) {
    // Create the file stream.
    std::ofstream fs(m_savePath);
    // Create the cereal archive.
    cereal::JSONOutputArchive jsonArchive(fs);

    jsonArchive(target);
    logger->coreLogger->debug("Main menu config saved to {}", m_savePath);
  }

  /**
   * @brief Helper method to get the input archive.
   * If the deserialization process failed, return the nullptr.
   *
   * @return std::shared_ptr<cereal::JSONInputArchive>
   */
  template <class T>
  void loadHelper(T& target) {
    namespace fs = std::filesystem;
    // Check if the file path exists.
    if (!fs::exists(fs::path(m_savePath))) {
      // Config file does not exist, create one.
      logger->coreLogger->warn(
          "Main menu config load failed, creating a new config.");
      save();

      return;
    }

    // Create the file stream.
    std::ifstream fstream(m_savePath);
    // Create the cereal archive.
    std::unique_ptr<cereal::JSONInputArchive> jsonArchive;

    try {
      jsonArchive = std::make_unique<cereal::JSONInputArchive>(fstream);
      (*jsonArchive)(target);
    } catch (const cereal::Exception& e) {
      // Handle deserialization failed.
      logger->coreLogger->error("Cereal JSON parser error.");
      save();
      logger->coreLogger->info("Override the old config with default value.");
      return;
    }

    logger->coreLogger->debug("Main menu config loaded from {}", m_savePath);
  }

 public:
  /**
   * @brief Construct a new Config object
   *
   * @param fileName the name of the config file.
   */
  Config(const std::string& fileName);

  /**
   * @brief Method to save the current config to the m_savePath.
   *
   */
  virtual void save() { throw std::logic_error("Not implemented"); }

  /**
   * @brief Method to load the current config from the m_savePath.
   */
  virtual void load() { throw std::logic_error("Not implemented"); }
};

}  // namespace hpaslt
