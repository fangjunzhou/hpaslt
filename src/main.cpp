#include <memory>

/* --------------------- Infrastructure --------------------- */
#include "logger/logger.h"
#include "commands/commands.h"
/* -------------------------- Core -------------------------- */
#include "core/audio_player/audio_player.h"
#include "core/audio_workspace/audio_workspace.h"
/* ------------------------ Rendering ----------------------- */
#include "window_manager/window_mgr.h"
#include "frontend/frontend.h"

int main(int argc, char const* argv[]) {
  hpaslt::logger->coreLogger->info("HPASLT started.");

  /* ---------------------------------------------------------- */
  /*                     Core Initialization                    */
  /* ---------------------------------------------------------- */

  /* --------------------- Infrastructure --------------------- */
  // Commands manager.
  hpaslt::logger->coreLogger->debug("Creating Commands manager.");
  hpaslt::Commands::getSingleton();

  /* -------------------------- Core -------------------------- */
  // Audio player.
  hpaslt::logger->coreLogger->debug("Creating AudioPlayer.");
  hpaslt::AudioPlayer::getSingleton();
  // Audio Workspace.
  hpaslt::logger->coreLogger->debug("Creating the main workspace.");
  hpaslt::AudioWorkspace::createAudioWorkspace("Main Workspace");
  hpaslt::logger->coreLogger->debug("Switching to the main workspace");
  hpaslt::AudioWorkspace::changeAudioWorkspace("Main Workspace");
  hpaslt::AudioWorkspace::registerConosleCommands();

  /* ------------------------ Rendering ----------------------- */
  // Window manager.
  hpaslt::logger->coreLogger->debug("Creating WindowManager.");
  hpaslt::WindowManager::getSingleton();
  /* ---------------------------------------------------------- */

  /* ---------------------------------------------------------- */
  /*                      Main Entry Point                      */
  /* ---------------------------------------------------------- */

  // Frontend entry point.
  hpaslt::logger->coreLogger->debug("Entering frontend entry point.");
  hpaslt::registerAllImGuiObjs();
  hpaslt::finishRegisterImGuiObjs();

  // WindowManager UI main loop.
  int exitRes = hpaslt::WindowManager::getSingleton().lock()->execute();

  /* ---------------------------------------------------------- */
  /*                        Core Cleanup                        */
  /* ---------------------------------------------------------- */

  /* ------------------------ Rendering ----------------------- */
  // Free the WindowManager singleton.
  hpaslt::WindowManager::freeSingleton();
  hpaslt::logger->coreLogger->debug("WindowManager singleton is freed.");

  /* -------------------------- Core -------------------------- */
  // Free AudioPlayer singleton.
  hpaslt::AudioPlayer::freeSingleton();
  hpaslt::logger->coreLogger->debug("AudioPlayer singleton is freed.");

  /* --------------------- Infrastructure --------------------- */
  // Commands manager.
  hpaslt::Commands::freeSingleton();
  hpaslt::logger->coreLogger->debug("Commands Manager singleton is freed.");
  /* ---------------------------------------------------------- */

  hpaslt::logger->coreLogger->info("HPASLT terminated.");

  return exitRes;
}
