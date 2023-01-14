#include "logger/logger.h"
#include "window_manager/window_mgr.h"

#include "frontend.h"
#include "menu/main_menu.h"
#include "status_bar/status_bar.h"

#include "console/console.h"

#include "imgui_example/imgui_example.h"

namespace hpaslt {

eventpp::CallbackList<void()> finishRegisterImGuiObjs;

void registerAllImGuiObjs() {
  hpaslt::logger->uiLogger->debug("Frontend entry point.");

  // Main Menu.
  hpaslt::WindowManager::getSingleton().lock()->setMainMenuBar(
      std::make_shared<MainMenu>());

  // Status bar.
  hpaslt::WindowManager::getSingleton().lock()->setMainStatusBar(
      std::make_shared<StatusBar>());

  /* -------------------------- Views ------------------------- */

  hpaslt::WindowManager::getSingleton().lock()->pushRenderObject(
      std::make_shared<Console>());

  /* -------------------------- Debug ------------------------- */

  // ImGui Example.
  hpaslt::WindowManager::getSingleton().lock()->pushRenderObject(
      std::make_shared<ImGuiExample>());
}

}  // namespace hpaslt