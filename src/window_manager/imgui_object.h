#pragma once

#include <eventpp/callbacklist.h>

#include <functional>
#include <string>

#include "common.h"
#include "logger/logger.h"

namespace hpaslt {

class ImGuiObject {
 private:
  /**
   * @brief The name of the ImGuiObject.
   *
   */
  std::string m_name;

 protected:
  /**
   * @brief If the current ImGuiObject should be rendered.
   *
   */
  bool m_enabled = true;

  /**
   * @brief ImGuiObject enable callback handle.
   *
   */
  eventpp::CallbackList<void(bool)>::Handle m_enableCallbackHandle;

  /**
   * @brief Callback handle for all ImGuiObjects finish register event.
   *
   */
  eventpp::CallbackList<void()>::Handle m_finishRegisterCallbackHandle;

  /**
   * @brief Register the onEnable callback list.
   *
   * @param callbackList
   */
  void setupEnableCallback(eventpp::CallbackList<void(bool)>& callbackList) {
    m_enabled = false;

    // Listen to the change event.
    m_enableCallbackHandle = callbackList.append(
        [this](bool enabled) { this->setEnabled(enabled); });
  }

  /**
   * @brief Reset and clear the callbacks.
   *
   * @param callbackList
   */
  void resetEnableCallback(eventpp::CallbackList<void(bool)>& callbackList) {
    callbackList.remove(m_enableCallbackHandle);
  }

  /**
   * @brief Callback function executed when all the ImGuiObjs are registered.
   *
   */
  virtual void onFinishRegisterImGuiObjs() {}

 public:
  /**
   * @brief Construct a new ImGuiObject object.
   *
   * @param name
   */
  ImGuiObject(std::string name) : m_name(name) {
    // When all the ImGuiObjects are registered, sync the config.
    m_finishRegisterCallbackHandle = finishRegisterImGuiObjs.append(
        [this]() { this->onFinishRegisterImGuiObjs(); });
  }

  /**
   * @brief Destroy the ImGuiObject object.
   *
   */
  virtual ~ImGuiObject() {
    finishRegisterImGuiObjs.remove(m_finishRegisterCallbackHandle);
  }

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
  void setEnabled(bool enabled) {
    logger->uiLogger->debug("{} is set to {}", m_name,
                            (enabled ? "enabled" : "disabled"));
    m_enabled = enabled;
  }

  /**
   * @brief Render method.
   *
   */
  virtual void render() {}
};

}  // namespace hpaslt
