#include <memory>

#include "logger/logger.h"
#include "window_manager/window_mgr.h"
#include "frontend/frontend.h"

int main(int argc, char const* argv[]) {
  hpaslt::logger->coreLogger->info("HPASLT started.");

  hpaslt::logger->coreLogger->debug("Creating WindowManager.");
  hpaslt::WindowManager::getSingleton();

  hpaslt::logger->coreLogger->debug("Entering frontend entry point.");
  hpaslt::registerAllImGuiObjs();
  hpaslt::finishRegisterImGuiObjs();

  // Join UI thread, get the execution result.
  int exitRes = hpaslt::WindowManager::getSingleton().lock()->execute();

  // Free the window manager singleton.
  hpaslt::WindowManager::freeSingleton();
  hpaslt::logger->coreLogger->debug("WindowManager singleton is freed.");

  hpaslt::logger->coreLogger->info("HPASLT terminated.");

  return exitRes;
}
