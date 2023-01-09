#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "imgui_example.h"

namespace hpaslt {

eventpp::CallbackList<void(bool)> ImGuiExample::s_onEnable;

ImGuiExample::ImGuiExample() : ImGuiObject("ImGui Example") {
  m_enabled = false;

  // Listen to the change event.
  m_enableCallbackHandle =
      s_onEnable.append([this](bool enabled) { this->setEnabled(enabled); });
}

ImGuiExample::~ImGuiExample() { s_onEnable.remove(m_enableCallbackHandle); }

void ImGuiExample::render() {
  // Show demo window.
  ImGui::ShowDemoWindow(nullptr);
}

}  // namespace hpaslt
