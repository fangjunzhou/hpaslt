#pragma once

#include <vector>
#include <mutex>

#include "core/audio_object/audio_object.h"
#include "window_manager/imgui_object.h"

namespace hpaslt {

struct AudioChannel {
  std::shared_ptr<std::vector<float>> wx;
  std::shared_ptr<std::vector<float>> wy;
  int size;
};

class WaveformWindow : public ImGuiObject {
 private:
  using LoadAudioCallback =
      eventpp::CallbackList<void(std::weak_ptr<AudioObject>)>;

  /**
   * @brief Mutex lock for m_audioObj.
   *
   */
  std::mutex m_audioMutex;

  /**
   * @brief The current playing audio object.
   *
   */
  std::shared_ptr<AudioObject> m_audioObj;

  /**
   * @brief The handle of audio loaded callback.
   *
   */
  LoadAudioCallback::Handle m_audioLoadedHandle;

  /* ----------------------- Audio Data ----------------------- */

  std::vector<AudioChannel> m_audioChannels;
  int m_channelNum;
  int m_sampleRate;
  int m_sampleSize;

 public:
  /**
   * @brief callback event when open the window from other place.
   *
   */
  static eventpp::CallbackList<void(bool)> s_onEnable;

  /**
   * @brief Construct a new WaveformWindow object.
   *
   */
  WaveformWindow();

  /**
   * @brief Destroy the WaveformWindow object.
   *
   */
  ~WaveformWindow();

  virtual void render() override;
};

}  // namespace hpaslt
