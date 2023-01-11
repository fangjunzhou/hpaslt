#include "console.h"

namespace hpaslt {

eventpp::CallbackList<void(bool)> Console::s_onEnable;

Console::Console() : ImGuiObject("Console") {
  m_enabled = false;

  // Listen to the change event.
  m_enableCallbackHandle =
      s_onEnable.append([this](bool enabled) { this->setEnabled(enabled); });
}

Console::~Console() { s_onEnable.remove(m_enableCallbackHandle); }

void Console::render() { m_consoleWindow.Draw(); }

}  // namespace hpaslt
