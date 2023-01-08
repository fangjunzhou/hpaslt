#include "config.h"

namespace hpaslt {

const std::string Config::m_baseDir = "config";

void Config::checkAndCreateDir(const std::filesystem::path& fullPath) {
  namespace fs = std::filesystem;
  // Get the parent path of the file and create it.
  auto parentPath = fullPath.parent_path();
  bool created = fs::create_directories(parentPath);

  if (created) {
    logger->coreLogger->info("Directory created for config object at {}.",
                             fullPath.string());
  } else {
    logger->coreLogger->info(
        "Directory not created for config object at {}, the directory may "
        "already exist.",
        fullPath.string());
  }
}

Config::Config(const std::string& fileName) {
  namespace fs = std::filesystem;
  auto fullPath = fs::path(m_baseDir) / fs::path(fileName);
  checkAndCreateDir(fullPath);

  m_savePath = fullPath.string();

  logger->coreLogger->info("Config object created at {}", m_savePath);
}

}  // namespace hpaslt
