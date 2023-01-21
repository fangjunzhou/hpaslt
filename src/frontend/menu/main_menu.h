#pragma once

#include <eventpp/callbacklist.h>
#include <nfd.h>
#include <future>

#include "serialization/main_menu/main_menu_config.h"

#include "window_manager/imgui_object.h"

using namespace std::chrono_literals;

namespace hpaslt {

class MainMenu : public ImGuiObject {
 private:
  std::unique_ptr<MainMenuConfig> m_config;

  /* -------------------------- File -------------------------- */

  std::future<std::string> m_audioFilePath;

  /* -------------------------- Views ------------------------- */

  bool m_showConsole = false;

  /* -------------------------- Debug ------------------------- */

  // If ImGuiExample window is displayed.
  bool m_showExample = false;

  /**
   * @brief Open audio file on another thread.
   *
   * @return nfdchar_t* file path.
   */
  std::string openAudioFile();

 protected:
  virtual void onFinishRegisterImGuiObjs() override;

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
