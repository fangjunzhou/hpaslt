#pragma once

#include <eventpp/callbacklist.h>
#include <imgui_console/imgui_console.h>

#include "window_manager/window_mgr.h"

namespace hpaslt {

/**
 * @brief Custom console class inherit from ImGuiConsole.
 *
 */
class ConsoleWindow : public ImGuiConsole {
  // TODO: Customize the ImGuiConsole.
};

class Console : public ImGuiObject {
 private:
  ConsoleWindow m_consoleWindow;

 public:
  /**
   * @brief callback event when open the window from other place.
   *
   */
  static eventpp::CallbackList<void(bool)> s_onEnable;

  Console();

  ~Console();

  virtual void render() override;
};

}  // namespace hpaslt
