#pragma once

#include <eventpp/callbacklist.h>

namespace hpaslt {

/**
 * @brief Callback function when all the ImGuiObjects are registered.
 *
 */
extern eventpp::CallbackList<void()> finishRegisterImGuiObjs;

}  // namespace hpaslt
