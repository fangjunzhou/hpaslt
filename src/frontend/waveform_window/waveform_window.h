#pragma once

#include "window_manager/imgui_object.h"

namespace hpaslt {

class WaveformWindow : public ImGuiObject {
 public:
  /**
   * @brief callback event when open the window from other place.
   *
   */
  static eventpp::CallbackList<void(bool)> s_onEnable;

  WaveformWindow() : ImGuiObject("Waveform") {
    setupEnableCallback(s_onEnable);
  }

  ~WaveformWindow() { resetEnableCallback(s_onEnable); }

  virtual void render() override;
};

}  // namespace hpaslt
