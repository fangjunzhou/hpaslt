#pragma once

#include "serialization/project_settings/project_settings_config.h"
#include "window_manager/imgui_object.h"

namespace hpaslt {

class ProjectSettings : public ImGuiObject {
private:
  std::shared_ptr<ProjectSettingsConfig> m_config;

public:
  /**
   * @brief callback event when open the window from other place.
   *
   */
  static eventpp::CallbackList<void(bool)> s_onEnable;

  /**
   * @brief Construct a new ProjectSettings object.
   *
   */
  ProjectSettings();

  /**
   * @brief Destroy the ProjectSettings object.
   *
   */
  ~ProjectSettings();

  virtual void render() override;
};

} // namespace hpaslt
