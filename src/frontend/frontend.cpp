#include "logger/logger.h"
#include "window_manager/window_mgr.h"

#include "frontend.h"
#include "menu/main_menu.h"
#include "imgui_example/imgui_example.h"

namespace hpaslt {

void registerAllImGuiObjs() {
  hpaslt::logger->uiLogger->debug("Frontend entry point.");

  // Main Menu.
  hpaslt::windowMgr->pushRenderObject(std::make_shared<MainMenu>());

  // ImGui Example.
  hpaslt::windowMgr->pushRenderObject(std::make_shared<ImGuiExample>());
}

}  // namespace hpaslt