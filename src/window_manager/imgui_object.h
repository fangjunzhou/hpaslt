#pragma once

namespace hpaslt {

class ImGuiObject {
 private:
  bool m_enabled = true;

 public:
  /**
   * @brief Get the enable status of the ImGuiObject.
   *
   * @return true when the ImGuiObject should be rendered.
   * @return false when the ImGuiObject should be skipped.
   */
  bool getEnabled() { return m_enabled; }

  /**
   * @brief Set the ImGuiObject render status.
   *
   * @param enabled if the ImGuiObject should be enabled.
   */
  void setEnabled(bool enabled) { m_enabled = enabled; }

  /**
   * @brief Render method.
   *
   */
  virtual void render() {}
};

}  // namespace hpaslt
