#include <filesystem>

#include "logger.h"

namespace hpaslt {

std::unique_ptr<Logger> logger = std::make_unique<Logger>("log");

Logger::Logger(std::string logDirPath) {
  namespace fs = std::filesystem;
  auto coreSinks =
      createSinks((fs::path(logDirPath) / fs::path("core.log")).string());
  coreLogger = std::make_shared<spdlog::logger>("core", coreSinks.begin(),
                                                coreSinks.end());

  auto uiSinks =
      createSinks((fs::path(logDirPath) / fs::path("ui.log")).string());
  uiLogger =
      std::make_shared<spdlog::logger>("ui", uiSinks.begin(), uiSinks.end());
}

Logger::~Logger() {}

std::vector<spdlog::sink_ptr> Logger::createSinks(std::string filePath) {
  std::vector<spdlog::sink_ptr> sinks;
  // Create stdout sink and file sink.
  sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
  sinks.push_back(
      std::make_shared<spdlog::sinks::basic_file_sink_mt>(filePath));
  // TODO: Add ImGui console sink.

  return sinks;
}

}  // namespace hpaslt