#pragma once

#include <cereal/archives/json.hpp>
#include <cereal/details/helpers.hpp>
#include <spdlog/spdlog.h>
#include <imgui.h>
#include <implot.h>

#include "logger/logger.h"
#include "serialization/config.h"

namespace hpaslt {

class ProjectSettingsConfig : public Config {
 private:
  static std::shared_ptr<ProjectSettingsConfig> s_projectSettingsConfig;

 public:
  static std::shared_ptr<ProjectSettingsConfig> getSingleton() {
    if (!s_projectSettingsConfig) {
      s_projectSettingsConfig =
          std::make_shared<ProjectSettingsConfig>("project_settings.json");
    }

    return s_projectSettingsConfig;
  }
  /* --------------------------- Log -------------------------- */

  spdlog::level::level_enum logLevel;

  /* ------------------------- ImPlot ------------------------- */

  ImGuiMouseButton panButton;
  ImGuiMouseButton timeButton;

  ProjectSettingsConfig(std::string fileName)
      : Config(fileName),
        logLevel(spdlog::level::info),
        panButton(ImGuiMouseButton_Middle),
        timeButton(ImGuiMouseButton_Left) {}

  template <class Archive>
  void serialize(Archive& archive) {
    archive(CEREAL_NVP(logLevel));
    archive(CEREAL_NVP(panButton), CEREAL_NVP(timeButton));
  }

  virtual void save() override { saveHelper(*this); }

  virtual void load() override {
    loadHelper(*this);
    /* --------------------------- Log -------------------------- */

    logger->setLogLevel(logLevel);

    /* ------------------------- ImPlot ------------------------- */

    ImPlot::GetInputMap().Pan = panButton;
  }
};

}  // namespace hpaslt
