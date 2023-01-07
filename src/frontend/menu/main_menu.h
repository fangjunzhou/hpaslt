#pragma once

#include "window_manager/imgui_object.h"

namespace hpaslt {

class MainMenu : public ImGuiObject {
 private:
  // If ImGuiExample window is displayed.
  bool m_showExample = false;

 public:
  MainMenu() : ImGuiObject("Main Menu") {}
  virtual void render() override;
};

}  // namespace hpaslt
