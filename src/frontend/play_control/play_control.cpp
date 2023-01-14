#include <imgui.h>

#include <IconsMaterialDesign.h>

#include "play_control.h"

namespace hpaslt {

PlayControl::PlayControl() : ImGuiObject("MainPlayConstrol") {}

PlayControl::~PlayControl() {}

void PlayControl::render() {
  // Status bar window.
  const ImGuiViewport* viewport = ImGui::GetMainViewport();
  ImVec2 statusPos = viewport->Pos;
  statusPos.y += ImGui::GetFrameHeight();
  ImGui::SetNextWindowPos(statusPos);
  // Resize the work space for status bar.
  ImVec2 workSize = viewport->Size;
  workSize.y = ImGui::GetFrameHeight();
  ImGui::SetNextWindowSize(workSize);
  ImGui::SetNextWindowViewport(viewport->ID);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, workSize);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
  // Disable padding.
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

  // Add menu bar flag and disable everything else
  // Disable docking to other dockspace.
  ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
  // Fix window.
  window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
                  ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
  // Disable nav focus.
  window_flags |=
      ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
  // Enable menu bar.
  window_flags |= ImGuiWindowFlags_MenuBar;

  ImGui::Begin("PlayControl", nullptr, window_flags);
  ImGui::PopStyleVar(4);

  if (ImGui::BeginMenuBar()) {
    ImGui::Text("Play Control");
    ImGui::EndMenuBar();
  }

  ImGui::End();
}

}  // namespace hpaslt
