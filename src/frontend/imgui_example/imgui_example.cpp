#include "imgui_example.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <implot.h>

#include "commands/commands.h"

namespace hpaslt {

eventpp::CallbackList<void(bool)> ImGuiExample::s_onEnable;

void ImGuiExample::render() {
  // Show demo window.
  ImGui::ShowDemoWindow(nullptr);
  // Show ImPlot demo window.
  ImPlot::ShowDemoWindow(nullptr);
}

}  // namespace hpaslt
