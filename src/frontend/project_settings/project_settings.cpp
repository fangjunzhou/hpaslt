#include "project_settings.h"

#include <imgui.h>
#include <implot.h>

#include "frontend/common/tooltip.h"

namespace hpaslt {

eventpp::CallbackList<void(bool)> ProjectSettings::s_onEnable;

ProjectSettings::ProjectSettings() : ImGuiObject("Project Settings") {
  // Setup window enable callback.
  setupEnableCallback(s_onEnable);
  m_config = ProjectSettingsConfig::getSingleton();
  // Try to load the config.
  m_config->load();
}

ProjectSettings::~ProjectSettings() { resetEnableCallback(s_onEnable); }

void ProjectSettings::render() {
  // Init window properties.
  const ImGuiWindowFlags windowFlags = ImGuiWindowFlags_None;
  ImGui::SetNextWindowSize(ImVec2(500, 440), ImGuiCond_FirstUseEver);
  ImGui::Begin("Project Settings", nullptr, windowFlags);

  ImGuiTabBarFlags tabBarFlags = ImGuiTabBarFlags_None;
  if (ImGui::BeginTabBar("SettingsTabBar", tabBarFlags)) {
    // Log tab.
    if (ImGui::BeginTabItem("Log Settings")) {
      // Log level.
      const std::unordered_map<spdlog::level::level_enum, std::string>
          logLevels = {{spdlog::level::off, "Off"},
                       {spdlog::level::critical, "Critical"},
                       {spdlog::level::err, "Error"},
                       {spdlog::level::warn, "Warn"},
                       {spdlog::level::info, "Info"},
                       {spdlog::level::debug, "Debug"},
                       {spdlog::level::trace, "Trace"}};
      const char* logLevelPreview = logLevels.at(m_config->logLevel).c_str();
      if (ImGui::BeginCombo("Log Level", logLevelPreview)) {
        for (auto& pair : logLevels) {
          const bool selected = (pair.first == m_config->logLevel);
          if (ImGui::Selectable(pair.second.c_str(), selected)) {
            // Update log level.
            logger->setLogLevel(pair.first);
            // Save log level config.
            m_config->logLevel = pair.first;
            m_config->save();
          }
          if (selected) ImGui::SetItemDefaultFocus();
        }

        ImGui::EndCombo();
      }

      ImGui::EndTabItem();
    }

    // ImPlot tab.
    if (ImGui::BeginTabItem("ImPlot Settings")) {
      // Pan button.
      const std::unordered_map<ImGuiMouseButton, std::string> panButtons = {
          {ImGuiMouseButton_Left, "Left"},
          {ImGuiMouseButton_Middle, "Middle"},
          {ImGuiMouseButton_Right, "Right"}};
      const char* panButtonPreview = panButtons.at(m_config->panButton).c_str();
      if (ImGui::BeginCombo("Pan Mouse Button", panButtonPreview)) {
        for (auto& pair : panButtons) {
          const bool selected = (pair.first == m_config->panButton);
          if (ImGui::Selectable(pair.second.c_str(), selected)) {
            // Update pan button.
            ImPlotInputMap& map = ImPlot::GetInputMap();
            map.Pan = pair.first;
            // Save pan button.
            m_config->panButton = pair.first;
            m_config->save();
          }
          if (selected) ImGui::SetItemDefaultFocus();
        }

        ImGui::EndCombo();
      }

      // Time button.
      const std::unordered_map<ImGuiMouseButton, std::string> timeButtons = {
          {ImGuiMouseButton_Left, "Left"},
          {ImGuiMouseButton_Middle, "Middle"},
          {ImGuiMouseButton_Right, "Right"}};
      const char* timeButtonPreview =
          timeButtons.at(m_config->timeButton).c_str();
      if (ImGui::BeginCombo("Time Mouse Button", timeButtonPreview)) {
        for (auto& pair : timeButtons) {
          const bool selected = (pair.first == m_config->timeButton);
          if (ImGui::Selectable(pair.second.c_str(), selected)) {
            // Save pan button.
            m_config->timeButton = pair.first;
            m_config->save();
          }
          if (selected) ImGui::SetItemDefaultFocus();
        }

        ImGui::EndCombo();
      }

      ImGui::EndTabItem();
    }

    // Advance settings.
    if (ImGui::BeginTabItem("Advance Settings")) {
      /* ---------------------- Audio Stream ---------------------- */
      ImGui::Text("Audio Stream Settings");

      // Audio stream frame per buffer.
      if (ImGui::DragInt("Audio Stream Frame per Buffer",
                         &(m_config->audioStreamFPB), 1, 1024, 65536)) {
      }
      if (ImGui::IsItemDeactivated()) {
        m_config->save();
      }
      ImGui::SameLine();
      Tooltip::helpMarker(
          "This settings change the buffer size of the underlying audio data "
          "structure. Smaller buffer size will lead to more smooth play "
          "control. But this may also lead to unacceptable lag on old "
          "machines.");

      ImGui::EndTabItem();
    }

    ImGui::EndTabBar();
  }

  ImGui::End();
}

}  // namespace hpaslt
