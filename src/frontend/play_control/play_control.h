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
  // Current playing time.
  float m_currTime;
  // Slider playing time.
  float m_sliderTime;
  // If synchronizing slider time with playing time.
  bool m_syncSliderTime;
  // Total time of the audio clip.
  float m_totalTime;
  eventpp::CallbackList<void(float, float)>::Handle
      m_onPlayingTimeChangedHandle;

  /**
   * @brief Helper method for switching from play and pause state.
   *
   */
  void playPauseSwitch();

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
