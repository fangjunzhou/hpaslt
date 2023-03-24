#pragma once

#include <eventpp/callbacklist.h>

namespace hpaslt {

/**
 * @brief Initialize frontend.
 *
 */
void frontendInit();

/**
 * @brief Terminate frontend.
 *
 */
void frontendTerminate();

/**
 * @brief Entry point of the frontend.
 *
 * This function must be called after hpaslt::windowMgr is initialized.
 *
 */
void registerAllImGuiObjs();

} // namespace hpaslt
