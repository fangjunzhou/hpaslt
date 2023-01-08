#pragma once

#include "serialization/main_menu/main_menu_config.h"

#include "window_manager/imgui_object.h"

namespace hpaslt {

class MainMenu : public ImGuiObject {
 private:
  std::unique_ptr<MainMenuConfig> m_config;

  // If ImGuiExample window is displayed.
  bool m_showExample = false;

 public:
  MainMenu() : ImGuiObject("Main Menu") {
    m_config = std::make_unique<MainMenuConfig>("main_menu.json");
    m_config->save();
  }

  virtual void render() override;
};

}  // namespace hpaslt
