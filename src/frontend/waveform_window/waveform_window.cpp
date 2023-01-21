#include <imgui.h>
#include <implot.h>

#include "waveform_window.h"

namespace hpaslt {

eventpp::CallbackList<void(bool)> WaveformWindow::s_onEnable;

void WaveformWindow::render() {
  // Init window properties.
  const ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar;
  ImGui::SetNextWindowSize(ImVec2(500, 440), ImGuiCond_FirstUseEver);
  ImGui::Begin("Waveform", nullptr, windowFlags);

  ImGui::End();
}

}  // namespace hpaslt
