#include "commands.h"

namespace hpaslt {

namespace Commands {

/**
 * @brief The external console system.
 *
 */
csys::System* s_consoleSystem;

using SetConsoleCallback = eventpp::CallbackList<void(csys::System*)>;
/**
 * @brief Callback list when console system is updated.
 *
 */
SetConsoleCallback s_onSetConsoleSystem;

using ResetConsoleCallback = eventpp::CallbackList<void()>;
/**
 * @brief Callback list when console system is reset.
 *
 */
ResetConsoleCallback s_onResetConsoleSystem;

SetConsoleCallback::Handle registerOnSetConsoleSystem(
    std::function<void(csys::System*)> func) {
  SetConsoleCallback::Handle handle = s_onSetConsoleSystem.append(func);

  if (s_consoleSystem) {
    func(s_consoleSystem);
  }

  return handle;
}

void unregisterOnSetConsoleSystem(SetConsoleCallback::Handle handle) {
  s_onSetConsoleSystem.remove(handle);
}

ResetConsoleCallback::Handle registerOnResetConsoleSystem(
    std::function<void()> func) {
  return s_onResetConsoleSystem.append(func);
}

void unregisterOnResetConsoleSystem(ResetConsoleCallback::Handle handle) {
  s_onResetConsoleSystem.remove(handle);
}

void setConsoleSystem(csys::System* consoleSystem) {
  s_consoleSystem = consoleSystem;
  s_onSetConsoleSystem(consoleSystem);
}

csys::System* getConsoleSystem() { return s_consoleSystem; }

void resetConsoleSystem() {
  s_consoleSystem = nullptr;
  s_onResetConsoleSystem();
}

}  // namespace Commands

}  // namespace hpaslt
