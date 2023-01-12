#pragma once

#include <map>

#include <eventpp/callbacklist.h>
#include <csys/csys.h>

namespace hpaslt {

namespace Commands {

using SetConsoleCallback = eventpp::CallbackList<void(csys::System*)>;

/**
 * @brief Register the callback function, when console system is set.
 * Call the function immediately if the console exists.
 *
 */
extern SetConsoleCallback::Handle registerOnSetConsoleSystem(
    std::function<void(csys::System*)> func);

/**
 * @brief Unregister a set console system callback handle.
 *
 * @param handle
 */
extern void unregisterOnSetConsoleSystem(SetConsoleCallback::Handle handle);

using ResetConsoleCallback = eventpp::CallbackList<void()>;
/**
 * @brief Register the callback function when console system is reset.
 *
 */
extern ResetConsoleCallback::Handle registerOnResetConsoleSystem(
    std::function<void()> func);

/**
 * @brief Unregister a reset console system callback handle.
 *
 * @param handle
 */
extern void unregisterOnResetConsoleSystem(ResetConsoleCallback::Handle handle);

/**
 * @brief Set the static console system to an external system.
 *
 * @param consoleSystem
 */
extern void setConsoleSystem(csys::System* consoleSystem);

/**
 * @brief Get the Console System object
 *
 * @return csys::System*
 */
extern csys::System* getConsoleSystem();

/**
 * @brief Reset the console system to nullptr.
 *
 */
extern void resetConsoleSystem();
};  // namespace Commands

}  // namespace hpaslt
