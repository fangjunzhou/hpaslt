#include "main_menu.h"

#include <IconsMaterialDesign.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <chrono>

#include "core/audio_workspace/audio_workspace.h"
#include "frontend/console/console.h"
#include "frontend/frontend.h"
#include "frontend/imgui_example/imgui_example.h"
#include "frontend/project_settings/project_settings.h"
#include "frontend/spectrogram_window/spectrogram_window.h"
#include "frontend/waveform_window/waveform_window.h"
#include "logger/logger.h"

namespace hpaslt {

std::string MainMenu::openAudioFile() {
  std::string res = "";

  // Async open file.
  nfdchar_t *outPath;
  nfdfilteritem_t filterItem[1] = {{"WAV Audio", "wav"}};
  nfdresult_t result = NFD_OpenDialog(&outPath, filterItem, 1, NULL);

  if (result == NFD_OKAY) {
    res = std::string(outPath);
    logger->coreLogger->trace("File opened at {}", res);
    NFD_FreePath(outPath);
  } else if (result == NFD_CANCEL) {
    logger->coreLogger->trace("File opened canceled");
  } else {
    logger->coreLogger->error("NativeFileDialog error: {}",
                              std::string(NFD_GetError()));
  }

  return res;
}

void MainMenu::onFinishRegisterImGuiObjs() {
  /* -------------------------- Views ------------------------- */

  m_showWaveform = m_config->showWaveform;
  WaveformWindow::s_onEnable(m_showWaveform);
  m_showSpectrogram = m_config->showSpectrogram;
  SpectrogramWindow::s_onEnable(m_showSpectrogram);
  m_showConsole = m_config->showConsole;
  Console::s_onEnable(m_showConsole);

  /* -------------------------- Debug ------------------------- */

  m_showExample = m_config->showExample;
  ImGuiExample::s_onEnable(m_showExample);
}

MainMenu::MainMenu() : ImGuiObject("MainMenuBar") {
  m_config = std::make_unique<MainMenuConfig>("main_menu.json");
  // Try to load the config.
  m_config->load();
}

MainMenu::~MainMenu() {}

void MainMenu::render() {
  if (ImGui::BeginMainMenuBar()) {
    if (ImGui::BeginMenu(ICON_MD_FOLDER " File")) {
      // Open audio file.
      if (ImGui::MenuItem(ICON_MD_FILE_OPEN " Open Audio File", "CTRL+O")) {
#if (PLATFORM == PLATFORM_WINDOWS)
        // Start a new thread for open audio file dialog.
        m_audioFilePath =
            std::async(std::launch::async, &MainMenu::openAudioFile, this);
#elif (PLATFORM == PLATFORM_LINUX)
        // Start a new thread for open audio file dialog.
        m_audioFilePath =
            std::async(std::launch::async, &MainMenu::openAudioFile, this);
#elif (PLATFORM == PLATFORM_MAC)
        std::promise<std::string> filePathPromise;
        m_audioFilePath = filePathPromise.get_future();
        filePathPromise.set_value(openAudioFile());
#endif
      }

      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu(ICON_MD_EDIT " Edit")) {
      if (ImGui::MenuItem("Undo", "CTRL+Z")) {
      }
      if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {
      } // Disabled item
      ImGui::Separator();
      if (ImGui::MenuItem("Cut", "CTRL+X")) {
      }
      if (ImGui::MenuItem("Copy", "CTRL+C")) {
      }
      if (ImGui::MenuItem("Paste", "CTRL+V")) {
      }
      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu(ICON_MD_WEB_ASSET " Views")) {
      if (ImGui::MenuItem(ICON_MD_GRAPHIC_EQ " Waveform Window", nullptr,
                          &m_showWaveform)) {
        WaveformWindow::s_onEnable(m_showWaveform);
        // Save the config.
        m_config->showWaveform = m_showWaveform;
        m_config->save();
      }

      if (ImGui::MenuItem(ICON_MD_GRAPHIC_EQ " Spectrogram Window", nullptr,
                          &m_showSpectrogram)) {
        SpectrogramWindow::s_onEnable(m_showSpectrogram);
        // Save the config.
        m_config->showSpectrogram = m_showSpectrogram;
        m_config->save();
      }

      ImGui::Separator();

      if (ImGui::MenuItem(ICON_MD_TERMINAL " Console", nullptr,
                          &m_showConsole)) {
        Console::s_onEnable(m_showConsole);
        // Save the config.
        m_config->showConsole = m_showConsole;
        m_config->save();
      }

      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu(ICON_MD_SETTINGS " HPASLT")) {
      if (ImGui::MenuItem(ICON_MD_BUG_REPORT " ImGui Example", nullptr,
                          &m_showExample)) {
        ImGuiExample::s_onEnable(m_showExample);
        // Save the config.
        m_config->showExample = m_showExample;
        m_config->save();
      }

      ImGui::Separator();

      if (ImGui::MenuItem(ICON_MD_SETTINGS " Project Settings", nullptr,
                          &m_showProjectSettings)) {
        ProjectSettings::s_onEnable(m_showProjectSettings);
      }

      ImGui::EndMenu();
    }

    ImGui::EndMainMenuBar();
  }

  /* -------------------- File Open Handle -------------------- */

  if (m_audioFilePath.valid()) {
    std::future_status audioFileStatus = m_audioFilePath.wait_for(0s);
    if (audioFileStatus == std::future_status::ready) {
      std::string path = m_audioFilePath.get();
      if (path.length() == 0) {
        logger->coreLogger->warn("File not opened.");
      } else {
        logger->coreLogger->debug("Opening file at {}", path);
        std::shared_ptr<AudioWorkspace> currWorkspace =
            AudioWorkspace::getSingleton().lock();
        currWorkspace->loadAudioFile(path);
      }
    }
  }
}

} // namespace hpaslt
