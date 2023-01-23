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
 public:
  /* --------------------------- Log -------------------------- */

  spdlog::level::level_enum logLevel;

  /* ------------------------- ImPlot ------------------------- */

  ImGuiMouseButton panButton;

  ProjectSettingsConfig(std::string fileName)
      : Config(fileName),
        logLevel(spdlog::level::info),
        panButton(ImGuiMouseButton_Middle) {}

  template <class Archive>
  void serialize(Archive& archive) {
    archive(CEREAL_NVP(logLevel));
    archive(CEREAL_NVP(panButton));
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
