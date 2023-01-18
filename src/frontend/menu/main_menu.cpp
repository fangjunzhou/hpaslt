#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <IconsMaterialDesign.h>

#include "main_menu.h"
#include "logger/logger.h"

#include "frontend/imgui_example/imgui_example.h"
#include "frontend/console/console.h"
#include "frontend/frontend.h"

namespace hpaslt {

MainMenu::MainMenu() : ImGuiObject("MainMenuBar") {
  namespace fs = std::filesystem;
  m_config = std::make_unique<MainMenuConfig>("main_menu.json");
  // Try to load the config.
  m_config->load();

  // When all the ImGuiObjects are registered, sync the config.
  m_finishRegisterCallbackHandle = finishRegisterImGuiObjs.append([&]() {
    m_showExample = m_config->showExample;
    ImGuiExample::s_onEnable(m_showExample);
    m_showConsole = m_config->showConsole;
    Console::s_onEnable(m_showConsole);
  });
}

MainMenu::~MainMenu() {
  finishRegisterImGuiObjs.remove(m_finishRegisterCallbackHandle);
}

void MainMenu::render() {
  if (ImGui::BeginMainMenuBar()) {
    if (ImGui::BeginMenu(ICON_MD_FOLDER " File")) {
      // Open audio file.
      if (ImGui::MenuItem("Open Audio File", "CTRL+O")) {
        // TODO: Async open file.
        nfdchar_t *outPath;
        nfdfilteritem_t filterItem[2] = {{"Source code", "c,cpp,cc"},
                                         {"Headers", "h,hpp"}};
        nfdresult_t result = NFD_OpenDialog(&outPath, filterItem, 2, NULL);
        if (result == NFD_OKAY) {
          logger->coreLogger->debug("File opened at {}", std::string(outPath));
          NFD_FreePath(outPath);
        } else if (result == NFD_CANCEL) {
          logger->coreLogger->debug("File opened canceled");
        } else {
          logger->coreLogger->error("NativeFileDialog error: {}",
                                    std::string(NFD_GetError()));
        }
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
      if (ImGui::MenuItem("ImGui Example", nullptr, &m_showExample)) {
        ImGuiExample::s_onEnable(m_showExample);
        // Save the config.
        m_config->showExample = m_showExample;
        m_config->save();
      }

      ImGui::EndMenu();
    }

    ImGui::EndMainMenuBar();
  }
}

}  // namespace hpaslt