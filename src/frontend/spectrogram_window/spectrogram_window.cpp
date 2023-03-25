#include "window_manager/imgui_object.h"

#include <imgui.h>
#include <implot.h>

#include "spectrogram_window.h"

namespace hpaslt {

eventpp::CallbackList<void(bool)> SpectrogramWindow::s_onEnable;

SpectrogramWindow::SpectrogramWindow() : ImGuiObject("Spectrogram") {
  // Setup window enable callback.
  setupEnableCallback(s_onEnable);
}

SpectrogramWindow::~SpectrogramWindow() {
  // Reset window enable callback.
  resetEnableCallback(s_onEnable);
}

void SpectrogramWindow::render() {
  // Init window properties.
  const ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar;
  ImGui::SetNextWindowSize(ImVec2(500, 440), ImGuiCond_FirstUseEver);
  ImGui::Begin("Spectrogram", nullptr, windowFlags);

  ImGui::End();
}

} // namespace hpaslt
