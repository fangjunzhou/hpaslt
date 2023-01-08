#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "main_menu.h"
#include "logger/logger.h"

#include "frontend/imgui_example/imgui_example.h"

namespace hpaslt {

void MainMenu::render() {
  if (ImGui::BeginMainMenuBar()) {
    if (ImGui::BeginMenu("File")) {
      // TODO: Implement file load, save.
      // ImGui::MenuItem("Not Implemented", nullptr, false, false);
      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Edit")) {
      if (ImGui::MenuItem("Undo", "CTRL+Z")) {
      }
      if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {
      }  // Disabled item
      ImGui::Separator();
      if (ImGui::MenuItem("Cut", "CTRL+X")) {
      }
      if (ImGui::MenuItem("Copy", "CTRL+C")) {
      }
      if (ImGui::MenuItem("Paste", "CTRL+V")) {
      }
      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Debug")) {
      if (ImGui::MenuItem("ImGui Example", nullptr, &m_showExample)) {
        ImGuiExample::s_onEnable(m_showExample);
      }
      ImGui::EndMenu();
    }

    ImGui::EndMainMenuBar();
  }
}

}  // namespace hpaslt