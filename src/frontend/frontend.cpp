#include "logger/logger.h"
#include "window_manager/window_mgr.h"

#include "frontend.h"
#include "menu/main_menu.h"

namespace hpaslt {

void registerAllImGuiObjs() {
  hpaslt::logger->uiLogger->debug("Frontend entry point.");

  // Main Menu.
  hpaslt::windowMgr->pushRenderObject(std::make_shared<MainMenu>());
  hpaslt::logger->uiLogger->debug("MainMenu registered.");
}

}  // namespace hpaslt