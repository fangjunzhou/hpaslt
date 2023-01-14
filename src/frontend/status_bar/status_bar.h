#pragma once

#include "window_manager/imgui_object.h"

namespace hpaslt {

class StatusBar : public ImGuiObject {
 private:
 public:
  /**
   * @brief Construct a new Status Bar object
   *
   */
  StatusBar();

  /**
   * @brief Destroy the Status Bar object
   *
   */
  ~StatusBar();

  virtual void render() override;
};

}  // namespace hpaslt
