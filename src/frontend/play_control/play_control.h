#pragma once

#include "window_manager/imgui_object.h"

namespace hpaslt {

class PlayControl : public ImGuiObject {
 public:
  /**
   * @brief Construct a new Status Bar object
   *
   */
  PlayControl();

  /**
   * @brief Destroy the Status Bar object
   *
   */
  ~PlayControl();

  virtual void render() override;
};

}  // namespace hpaslt
