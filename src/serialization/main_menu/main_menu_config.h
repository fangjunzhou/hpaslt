#pragma once

#include <string>

#include <cereal/archives/json.hpp>

#include "serialization/config.h"

namespace hpaslt {

class MainMenuConfig : public Config {
 public:
  MainMenuConfig(std::string savePath) : Config(savePath) {}

  bool showExample;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(CEREAL_NVP(showExample));
  }

  virtual void save() override;

  virtual void load() override;
};

}  // namespace hpaslt
