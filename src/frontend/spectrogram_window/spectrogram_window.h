#include "core/audio_object/audio_object.h"

#include <GL/glew.h>
#include <memory>

#include "core/audio_spectrogram/audio_spectrogram.h"
#include "window_manager/imgui_object.h"

namespace hpaslt {

class SpectrogramWindow : public ImGuiObject {
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

  /* ----------------------- Spectrogram ---------------------- */

  std::shared_ptr<AudioSpectrogram> m_audioSpectrogram;

  /* -------------------- OpenGL Rendering -------------------- */

  /**
   * @brief The OpenGL texture id.
   */
  GLuint m_spectrogramTextureId;

public:
  /**
   * @brief callback event when open the window from other place.
   *
   */
  static eventpp::CallbackList<void(bool)> s_onEnable;

  /**
   * @brief Constructor of SpectrogramWindow.
   */
  SpectrogramWindow();

  /**
   * @brief Destructor of SpectrogramWindow.
   */
  ~SpectrogramWindow();

  virtual void render() override;
};

} // namespace hpaslt
