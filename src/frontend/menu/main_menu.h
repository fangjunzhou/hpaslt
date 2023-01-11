#pragma once

#include <eventpp/callbacklist.h>

#include "serialization/main_menu/main_menu_config.h"

#include "window_manager/imgui_object.h"

namespace hpaslt {

class MainMenu : public ImGuiObject {
 private:
  std::unique_ptr<MainMenuConfig> m_config;

  /* -------------------------- Views ------------------------- */

  bool m_showConsole = false;

  /* -------------------------- Debug ------------------------- */

  // If ImGuiExample window is displayed.
  bool m_showExample = false;

 public:
  /**
   * @brief Construct a new Main Menu object
   *
   */
  MainMenu();

  /**
   * @brief Destroy the Main Menu object
   *
   */
  ~MainMenu();

  virtual void render() override;
};

}  // namespace hpaslt
