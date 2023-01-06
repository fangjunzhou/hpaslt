#include <memory>

#include "logger/logger.h"
#include "window_manager/window_mgr.h"

int main(int argc, char const* argv[]) {
  hpaslt::logger->coreLogger->info("HPASLT Started.");

  hpaslt::logger->coreLogger->info("Creating WindowManager.");
  // Create the window manager, start the UI thread.
  std::shared_ptr<hpaslt::WindowManager> windowMgr;
  try {
    windowMgr = std::make_shared<hpaslt::WindowManager>();
  } catch (const std::system_error& e) {
    return EXIT_FAILURE;
  }

  // Join UI thread, get the execution result.
  int exitRes = windowMgr->execute();

  return exitRes;
}
