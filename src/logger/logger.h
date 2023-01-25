#pragma once

#include <sstream>
#include <string>
#include <vector>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/ostream_sink.h>

namespace hpaslt {

class Logger {
 private:
  /**
   * @brief The ostringstream for console sink.
   *
   */
  std::shared_ptr<std::ostringstream> m_consoleOutputStream;

  /**
   * @brief Create a sinks_init_list for logger.
   *
   * @param filePath the path of log file.
   * @return spdlog::sinks_init_list the sink list.
   */
  std::vector<spdlog::sink_ptr> createSinks(std::string filePath);

 public:
  /**
   * @brief Core logger.
   *
   */
  std::shared_ptr<spdlog::logger> coreLogger;

  /**
   * @brief Logger for UI thread.
   *
   */
  std::shared_ptr<spdlog::logger> uiLogger;

  /**
   * @brief Construct a new Logger object
   *
   * @param logDirPath the path to the log directory.
   */
  Logger(std::string logDirPath, spdlog::level::level_enum level,
         bool enableConsoleSink = false);

  /**
   * @brief Destroy the Logger object
   *
   */
  ~Logger();

  /**
   * @brief Get the Console ostringsteam object.
   *
   * @return std::weak_ptr<std::ostringstream> weak ptr to the console
   * ostringsteam.
   */
  std::weak_ptr<std::ostringstream> getConsoleOutputStream() {
    return m_consoleOutputStream;
  }

  /**
   * @brief Set the log level for all logger.
   *
   * @param logLevel
   */
  void setLogLevel(spdlog::level::level_enum logLevel);
};

extern std::unique_ptr<Logger> logger;

extern void initLogger(std::string workingDirectory);

extern void terminateLogger();

}  // namespace hpaslt