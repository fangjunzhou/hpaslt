#include <memory>

#include "logger/logger.h"
#include "window_manager/window_mgr.h"
#include "frontend/frontend.h"

int main(int argc, char const* argv[]) {
  hpaslt::logger->coreLogger->info("HPASLT started.");

  hpaslt::logger->coreLogger->debug("Creating WindowManager.");
  // Create the window manager, start the UI thread.
  try {
    hpaslt::windowMgr = std::make_shared<hpaslt::WindowManager>();
  } catch (const std::system_error& e) {
    return EXIT_FAILURE;
  }

  hpaslt::logger->coreLogger->debug("Entering frontend entry point.");
  hpaslt::registerAllImGuiObjs();

  // Join UI thread, get the execution result.
  int exitRes = hpaslt::windowMgr->execute();

  hpaslt::logger->coreLogger->info("HPASLT terminated.");

  return exitRes;
}
