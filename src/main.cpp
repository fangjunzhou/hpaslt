#include <memory>

#include "logger/logger.h"
#include "window_manager/window_mgr.h"
#include "frontend/frontend.h"
#include "commands/commands.h"

int main(int argc, char const* argv[]) {
  hpaslt::logger->coreLogger->info("HPASLT started.");

  /* ------------------- Core Initialization ------------------ */
  // Commands manager.
  hpaslt::Commands::getSingleton();
  // Window manager.
  hpaslt::logger->coreLogger->debug("Creating WindowManager.");
  hpaslt::WindowManager::getSingleton();
  /* ---------------------------------------------------------- */

  hpaslt::logger->coreLogger->debug("Entering frontend entry point.");
  hpaslt::registerAllImGuiObjs();
  hpaslt::finishRegisterImGuiObjs();

  // Join UI thread, get the execution result.
  int exitRes = hpaslt::WindowManager::getSingleton().lock()->execute();

  /* ---------------------- Core Cleanup ---------------------- */
  // Free the window manager singleton.
  hpaslt::WindowManager::freeSingleton();
  hpaslt::logger->coreLogger->debug("WindowManager singleton is freed.");
  // Commands manager.
  hpaslt::Commands::freeSingleton();
  /* ---------------------------------------------------------- */

  hpaslt::logger->coreLogger->info("HPASLT terminated.");

  return exitRes;
}
