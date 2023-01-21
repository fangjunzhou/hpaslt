#pragma once

#include <string>

#include <cereal/archives/json.hpp>
#include <cereal/details/helpers.hpp>

#include "serialization/config.h"

namespace hpaslt {

class MainMenuConfig : public Config {
 public:
  MainMenuConfig(std::string fileName) : Config(fileName) {}

  /* -------------------------- Views ------------------------- */

  bool showWaveform = false;
  bool showConsole = false;

  /* -------------------------- Debug ------------------------- */

  bool showExample = false;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(CEREAL_NVP(showWaveform), CEREAL_NVP(showConsole));
    archive(CEREAL_NVP(showExample));
  }

  virtual void save() override { saveHelper(*this); }

  virtual void load() override { loadHelper(*this); }
};

}  // namespace hpaslt
