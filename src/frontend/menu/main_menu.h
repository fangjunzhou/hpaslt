#pragma once

#include "window_manager/imgui_object.h"

namespace hpaslt {

class MainMenu : public ImGuiObject {
 public:
  virtual void render() override;
};

}  // namespace hpaslt
