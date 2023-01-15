#pragma once

#include <eventpp/callbacklist.h>

#include "window_manager/imgui_object.h"

namespace hpaslt {

class PlayControl : public ImGuiObject {
 private:
  /* ----------------- Current Playing Status ----------------- */
  bool m_isPlaying;
  eventpp::CallbackList<void(bool)>::Handle m_onPlayingStatusChangedHandle;
  /* ---------------------- Playing Time ---------------------- */
  float m_currTime;
  float m_totalTime;
  eventpp::CallbackList<void(float, float)>::Handle
      m_onPlayingTimeChangedHandle;

 public:
  /**
   * @brief Construct a new Status Bar object
   *
   */
  PlayControl();

  /**
   * @brief Destroy the Status Bar object
   *
   */
  ~PlayControl();

  virtual void render() override;
};

}  // namespace hpaslt
