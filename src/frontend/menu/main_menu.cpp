#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <IconsMaterialDesign.h>

#include <chrono>

#include "main_menu.h"
#include "logger/logger.h"

#include "frontend/waveform_window/waveform_window.h"
#include "frontend/console/console.h"

#include "frontend/imgui_example/imgui_example.h"
#include "frontend/frontend.h"

#include "core/audio_workspace/audio_workspace.h"

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
  m_showConsole = m_config->showConsole;
  Console::s_onEnable(m_showConsole);

  /* -------------------------- Debug ------------------------- */

  m_showExample = m_config->showExample;
  ImGuiExample::s_onEnable(m_showExample);
}

MainMenu::MainMenu() : ImGuiObject("MainMenuBar") {
  namespace fs = std::filesystem;
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
        // TODO: Implement sync file dialog on macos.
        m_audioFilePath = std::async(std::launch::async, openAudioFile, this);
      }

      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu(ICON_MD_EDIT " Edit")) {
      if (ImGui::MenuItem("Undo", "CTRL+Z")) {
      }
      if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {
      }  // Disabled item
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
      if (ImGui::MenuItem(ICON_MD_TERMINAL " Waveform Window", nullptr,
                          &m_showWaveform)) {
        WaveformWindow::s_onEnable(m_showWaveform);
        // Save the config.
        m_config->showWaveform = m_showWaveform;
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

    if (ImGui::BeginMenu(ICON_MD_BUG_REPORT " Debug")) {
      if (ImGui::MenuItem(ICON_MD_BUG_REPORT " ImGui Example", nullptr,
                          &m_showExample)) {
        ImGuiExample::s_onEnable(m_showExample);
        // Save the config.
        m_config->showExample = m_showExample;
        m_config->save();
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

}  // namespace hpaslt