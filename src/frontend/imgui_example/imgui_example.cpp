#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "imgui_example.h"
#include "commands/commands.h"

namespace hpaslt {

eventpp::CallbackList<void(bool)> ImGuiExample::s_onEnable;

ImGuiExample::ImGuiExample() : ImGuiObject("ImGui Example") {
  m_enabled = false;

  // Listen to the change event.
  m_enableCallbackHandle =
      s_onEnable.append([this](bool enabled) { this->setEnabled(enabled); });

  // Register open/close example window command.
  m_setConsoleHandle =
      Commands::registerOnSetConsoleSystem([](csys::System* system) {
        system->RegisterCommand(
            "setImGuiExample", "Enable or disable ImGui Example window.",
            [](bool enable) { s_onEnable(enable); }, csys::Arg<bool>("enable"));
      });
}

ImGuiExample::~ImGuiExample() {
  s_onEnable.remove(m_enableCallbackHandle);
  Commands::unregisterOnSetConsoleSystem(m_setConsoleHandle);
}

void ImGuiExample::render() {
  // Show demo window.
  ImGui::ShowDemoWindow(nullptr);
}

}  // namespace hpaslt
