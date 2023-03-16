#include "logger.h"

#include <ctime>
#include <filesystem>
#include <iostream>
#include <sstream>

namespace hpaslt {

std::unique_ptr<Logger> logger = nullptr;

void initLogger(std::string workingDirectory) {
  std::filesystem::path loggerDirPath =
      std::filesystem::path(workingDirectory) / "log";
  logger = std::make_unique<Logger>(loggerDirPath.string(),
                                    spdlog::level::trace, true);
}

void terminateLogger() { logger = nullptr; }

Logger::Logger(std::string logDirPath, spdlog::level::level_enum level,
               bool enableConsoleSink) {
  // Setup console ostringstream.
  if (enableConsoleSink) {
    m_consoleOutputStream = std::make_shared<std::ostringstream>();
  } else {
    m_consoleOutputStream = nullptr;
  }

  namespace fs = std::filesystem;

  auto t = std::time(nullptr);
  auto tm = std::localtime(&t);
  std::ostringstream ss;
  ss << std::put_time(tm, "%Y-%m-%d_%H-%M-%S");
  std::string currTime = ss.str();
  std::string fileName = currTime + ".log";

  auto logFullPath = (fs::path(logDirPath) / fs::path(fileName));
  std::cout << "Log file " << logFullPath.string() << " generated."
            << std::endl;

  auto sinks = createSinks(logFullPath.string());

  coreLogger =
      std::make_shared<spdlog::logger>("core", sinks.begin(), sinks.end());
  uiLogger = std::make_shared<spdlog::logger>("ui", sinks.begin(), sinks.end());

  coreLogger->set_level(level);
  uiLogger->set_level(level);
}

Logger::~Logger() {}

std::vector<spdlog::sink_ptr> Logger::createSinks(std::string filePath) {
  std::vector<spdlog::sink_ptr> sinks;
  // Create stdout sink and file sink.
  sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
  sinks.push_back(
      std::make_shared<spdlog::sinks::basic_file_sink_mt>(filePath));
  // Add ImGui console sink.
  if (m_consoleOutputStream) {
    sinks.push_back(std::make_shared<spdlog::sinks::ostream_sink_mt>(
        *m_consoleOutputStream));
  }

  return sinks;
}

void Logger::setLogLevel(spdlog::level::level_enum logLevel) {
  coreLogger->set_level(logLevel);
  uiLogger->set_level(logLevel);
}

}  // namespace hpaslt