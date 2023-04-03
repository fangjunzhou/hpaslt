#include "window_manager/imgui_object.h"

#include <GL/glew.h>
#include <imgui.h>
#include <implot.h>

#include "core/audio_workspace/audio_workspace.h"
#include "spectrogram_window.h"

namespace hpaslt {

eventpp::CallbackList<void(bool)> SpectrogramWindow::s_onEnable;

SpectrogramWindow::SpectrogramWindow() : ImGuiObject("Spectrogram") {
  // Setup window enable callback.
  setupEnableCallback(s_onEnable);

  // Steup audio loaded callback.
  m_audioLoadedHandle = AudioWorkspace::s_onAudioLoaded.append(
      [&](std::weak_ptr<AudioObject> audioObj) {
        logger->coreLogger->trace("SpectrogramWindow load new AudioObject.");

        // Load m_audioObj.
        m_audioMutex.lock();

        m_audioObj = audioObj.lock();

        m_audioMutex.unlock();
      });

  // Setup the texture.
  glGenTextures(1, &m_spectrogramTextureId);
  glBindTexture(GL_TEXTURE_2D, m_spectrogramTextureId);
  // Set the texture wrap.
  glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
  // Set the texture filter.
  glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                      GL_NEAREST_MIPMAP_NEAREST);
  glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                      GL_NEAREST_MIPMAP_NEAREST);
}

SpectrogramWindow::~SpectrogramWindow() {
  // Reset window enable callback.
  resetEnableCallback(s_onEnable);
  // Destroy the texture.
  glDeleteTextures(1, &m_spectrogramTextureId);
  // Remove audio loaded callback.
  AudioWorkspace::s_onAudioLoaded.remove(m_audioLoadedHandle);
}

void SpectrogramWindow::render() {
  // Init window properties.
  const ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar;
  ImGui::SetNextWindowSize(ImVec2(500, 440), ImGuiCond_FirstUseEver);
  ImGui::Begin("Spectrogram", nullptr, windowFlags);

  ImGui::End();
}

} // namespace hpaslt
