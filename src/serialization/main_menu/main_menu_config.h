#pragma once

#include <cereal/archives/json.hpp>
#include <cereal/details/helpers.hpp>
#include <string>

#include "serialization/config.h"

namespace hpaslt {

class MainMenuConfig : public Config {
public:
  /* -------------------------- Views ------------------------- */

  bool showWaveform = false;
  bool showConsole = false;

  /* -------------------------- Debug ------------------------- */

  bool showExample = false;

  MainMenuConfig(std::string fileName) : Config(fileName) {}

  template <class Archive> void serialize(Archive &archive) {
    archive(CEREAL_NVP(showWaveform), CEREAL_NVP(showConsole));
    archive(CEREAL_NVP(showExample));
  }

  virtual void save() override { saveHelper(*this); }

  virtual void load() override { loadHelper(*this); }
};

} // namespace hpaslt
