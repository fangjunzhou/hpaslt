#pragma once

#include <cereal/archives/json.hpp>
#include <cereal/details/helpers.hpp>
#include <spdlog/spdlog.h>

#include "logger/logger.h"
#include "serialization/config.h"

namespace hpaslt {

class ProjectSettingsConfig : public Config {
 public:
  /* --------------------------- Log -------------------------- */
  spdlog::level::level_enum logLevel;

  ProjectSettingsConfig(std::string fileName)
      : Config(fileName), logLevel(spdlog::level::info) {}

  template <class Archive>
  void serialize(Archive& archive) {
    archive(CEREAL_NVP(logLevel));
  }

  virtual void save() override { saveHelper(*this); }

  virtual void load() override {
    loadHelper(*this);
    logger->setLogLevel(logLevel);
  }
};

}  // namespace hpaslt
