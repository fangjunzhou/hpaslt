#pragma once

#include <csys/csys.h>
#include <eventpp/callbacklist.h>

#include "window_manager/imgui_object.h"

namespace hpaslt {

class ImGuiExample : public ImGuiObject {
public:
  /**
   * @brief callback event when open the window from other place.
   *
   */
  static eventpp::CallbackList<void(bool)> s_onEnable;

  ImGuiExample() : ImGuiObject("ImGui Example") {
    setupEnableCallback(s_onEnable);
  }

  ~ImGuiExample() { resetEnableCallback(s_onEnable); }

  virtual void render() override;
};

} // namespace hpaslt
