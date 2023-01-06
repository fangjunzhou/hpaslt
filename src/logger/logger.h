#pragma once

#include <string>
#include <vector>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace hpaslt {

class Logger {
 private:
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
  Logger(std::string logDirPath, spdlog::level::level_enum level);

  /**
   * @brief Destroy the Logger object
   *
   */
  ~Logger();
};

extern std::unique_ptr<Logger> logger;

}  // namespace hpaslt