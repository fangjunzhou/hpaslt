#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

auto consoleLogger = spdlog::stdout_color_mt("console");

int main(int argc, char const *argv[]) {
  consoleLogger->info("HPASLT Started.");
  return 0;
}
