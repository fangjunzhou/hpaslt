#pragma once

#include <eventpp/callbacklist.h>
#include <csys/csys.h>

#include "window_manager/imgui_object.h"

namespace hpaslt {

class ImGuiExample : public ImGuiObject {
 public:
  /**
   * @brief callback event when open the window from other place.
   *
   */
  static eventpp::CallbackList<void(bool)> s_onEnable;

  ImGuiExample();

  ~ImGuiExample();

  virtual void render() override;
};

}  // namespace hpaslt
