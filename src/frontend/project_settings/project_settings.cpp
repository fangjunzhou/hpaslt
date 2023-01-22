#include <imgui.h>

#include "project_settings.h"

namespace hpaslt {

eventpp::CallbackList<void(bool)> ProjectSettings::s_onEnable;

ProjectSettings::ProjectSettings() : ImGuiObject("Project Settings") {
  // Setup window enable callback.
  setupEnableCallback(s_onEnable);
  m_config = std::make_unique<ProjectSettingsConfig>("project_settings.json");
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
    // Log tab bar.
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

    ImGui::EndTabBar();
  }

  ImGui::End();
}

}  // namespace hpaslt
