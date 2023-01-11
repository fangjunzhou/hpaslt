#pragma once

#include <string>

#include <cereal/archives/json.hpp>
#include <cereal/details/helpers.hpp>

#include "serialization/config.h"

namespace hpaslt {

class MainMenuConfig : public Config {
 public:
  MainMenuConfig(std::string fileName) : Config(fileName) {}

  bool showExample = false;
  bool showConsole = false;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(CEREAL_NVP(showExample), CEREAL_NVP(showConsole));
  }

  virtual void save() override;

  virtual void load() override;
};

}  // namespace hpaslt
