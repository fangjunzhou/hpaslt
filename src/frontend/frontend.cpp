#include "logger/logger.h"
#include "window_manager/window_mgr.h"

#include "frontend.h"
#include "menu/main_menu.h"
#include "imgui_example/imgui_example.h"

namespace hpaslt {

eventpp::CallbackList<void()> finishRegisterImGuiObjs;

void registerAllImGuiObjs() {
  hpaslt::logger->uiLogger->debug("Frontend entry point.");

  // Main Menu.
  hpaslt::WindowManager::getSingleton()->pushRenderObject(
      std::make_shared<MainMenu>());

  // ImGui Example.
  hpaslt::WindowManager::getSingleton()->pushRenderObject(
      std::make_shared<ImGuiExample>());
}

}  // namespace hpaslt