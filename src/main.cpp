#include <filesystem>
#include <memory>
#include <pathfind.hpp>

/* --------------------- Infrastructure --------------------- */
#include "commands/commands.h"
#include "common/workspace_context.h"
#include "logger/logger.h"
/* -------------------------- Core -------------------------- */
#include "core/audio_player/audio_player.h"
#include "core/audio_workspace/audio_workspace.h"
/* ------------------------ Rendering ----------------------- */
#include "frontend/frontend.h"
#include "window_manager/window_mgr.h"

int main(int argc, char const* argv[]) {
  /* ------------------------- Context ------------------------ */
  // Setup working directory.
  std::filesystem::path executablePath = PathFind::FindExecutable();
  hpaslt::workspaceContext::hpasltWorkingDirectory =
      executablePath.parent_path().string();

  /* ------------------------- Logger ------------------------- */
  hpaslt::initLogger(hpaslt::workspaceContext::hpasltWorkingDirectory);
  hpaslt::logger->coreLogger->info(
      "HPASLT started at {}.",
      hpaslt::workspaceContext::hpasltWorkingDirectory);

  /* ---------------------------------------------------------- */
  /*                     Core Initialization                    */
  /* ---------------------------------------------------------- */

  /* --------------------- Infrastructure --------------------- */
  // Commands manager.
  hpaslt::logger->coreLogger->debug("Creating Commands manager.");
  hpaslt::Commands::getSingleton();

  /* -------------------------- Core -------------------------- */
  // Audio player.
  hpaslt::logger->coreLogger->debug("Initializaing AudioPlayer.");
  hpaslt::AudioPlayer::initAudioPlayer();
  // Audio Workspace.
  hpaslt::logger->coreLogger->debug("Creating the main workspace.");
  hpaslt::AudioWorkspace::getSingleton();
  hpaslt::logger->coreLogger->debug("Registering console commands.");
  hpaslt::AudioWorkspace::registerConosleCommands();

  /* ------------------------ Rendering ----------------------- */
  // Window manager.
  hpaslt::logger->coreLogger->debug("Creating WindowManager.");
  hpaslt::WindowManager::getSingleton();
  // Frontend.
  hpaslt::frontendInit();
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
  // Frontend.
  hpaslt::frontendTerminate();

  /* -------------------------- Core -------------------------- */
  // Cleanup audio workspaces.
  hpaslt::AudioWorkspace::freeSingleton();
  hpaslt::logger->coreLogger->debug("AudioWorkspace freed.");

  // Free AudioPlayer singleton.
  hpaslt::AudioPlayer::terminateAudioPlayer();
  hpaslt::logger->coreLogger->debug("AudioPlayer terminated.");

  /* --------------------- Infrastructure --------------------- */
  // Commands manager.
  hpaslt::Commands::freeSingleton();
  hpaslt::logger->coreLogger->debug("Commands Manager singleton is freed.");
  /* ---------------------------------------------------------- */

  hpaslt::logger->coreLogger->info("HPASLT terminated.");

  /* ------------------------- Logger ------------------------- */
  hpaslt::terminateLogger();

  return exitRes;
}
