#include <IconsMaterialDesign.h>
#include <implot.h>

#include <filesystem>

#include "common/workspace_context.h"
#include "logger/logger.h"
#include "window_mgr.h"

namespace hpaslt {

std::shared_ptr<WindowManager> WindowManager::s_windowMgr = nullptr;

void WindowManager::enableDockspace() {
  // Enable dock space.
  const ImGuiViewport* viewport = ImGui::GetMainViewport();
  ImVec2 workPos = viewport->Pos;
  // Leave space for menu + play control.
  workPos.y += 2 * ImGui::GetFrameHeight();
  ImGui::SetNextWindowPos(workPos);
  // Resize the work space for status bar.
  ImVec2 workSize = viewport->Size;
  // Leave space for status.
  workSize.y -= 3 * ImGui::GetFrameHeight();
  ImGui::SetNextWindowSize(workSize);
  ImGui::SetNextWindowViewport(viewport->ID);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
  // Disable padding.
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

  // Setup docking window flag.
  // Disable docking to other dockspace.
  ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
  // Fix window.
  window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
                  ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
  // Disable nav focus.
  window_flags |=
      ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

  // Dockspace flag.
  static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

  ImGui::Begin("DockSpace", nullptr, window_flags);
  ImGui::PopStyleVar(3);

  // Submit the DockSpace
  ImGuiIO& io = ImGui::GetIO();
  if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
    ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
  }

  ImGui::End();
}

WindowManager::WindowManager()
    : m_mainMenuBar(nullptr), m_mainStatusBar(nullptr), m_playControl(nullptr) {
  hpaslt::logger->uiLogger->debug("WindowManager created.");

  // Init GLFW.
  if (!glfwInit()) {
    hpaslt::logger->uiLogger->error("GLFW initialization failed!");
    throw std::runtime_error("GLFW initialization failed!");
  }
  hpaslt::logger->uiLogger->debug("GLFW initialization succeeded.");

  // Enable GLFW 4x MSAA.
  glfwWindowHint(GLFW_SAMPLES, 4);
  // Set OpenGL version.
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  // Mac compatability.
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  m_window = glfwCreateWindow(960, 540, "HPASLT", nullptr, nullptr);
  if (!m_window) {
    hpaslt::logger->uiLogger->error("GLFW window creation failed!");
    throw std::runtime_error("GLFW window creation failed!");
  }
  hpaslt::logger->uiLogger->debug("GLFW window creation succeeded.");

  glfwMakeContextCurrent(m_window);
  // Enable vsync.
  glfwSwapInterval(1);

  // Init GLEW.
  glewExperimental = true;
  if (glewInit() != GLEW_OK) {
    hpaslt::logger->uiLogger->error("GLEW initialization failed!");
    throw std::runtime_error("GLEW initialization failed!");
  }
  hpaslt::logger->uiLogger->debug("GLEW initialization succeeded.");

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImPlot::CreateContext();
  hpaslt::logger->uiLogger->debug("ImGui context created.");
  m_io = &ImGui::GetIO();
  m_io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  m_io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;
  m_io->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();

  // When viewports are enabled we tweak WindowRounding/WindowBg so platform
  // windows can look identical to regular ones.
  ImGuiStyle& style = ImGui::GetStyle();
  if (m_io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
    style.WindowRounding = 0.0f;
    style.Colors[ImGuiCol_WindowBg].w = 1.0f;
  }

  // Setup Platform/Renderer backends
  ImGui_ImplGlfw_InitForOpenGL(m_window, true);
  ImGui_ImplOpenGL3_Init("#version 150");

  // Load fonts here.
  std::filesystem::path fontPath =
      std::filesystem::path(hpaslt::workspaceContext::hpasltWorkingDirectory) /
      "fonts/ubuntu/Ubuntu-Regular.ttf";
  if (std::filesystem::exists(fontPath)) {
    m_io->Fonts->AddFontFromFileTTF(fontPath.c_str(), 16.0f);
  } else {
    logger->coreLogger->error("Font loaded failed.");
  }

  std::filesystem::path materialIconPath =
      std::filesystem::path(hpaslt::workspaceContext::hpasltWorkingDirectory) /
      "fonts/MaterialIcons-Regular.ttf";
  if (std::filesystem::exists(materialIconPath) &&
      std::filesystem::exists(fontPath)) {
    // Merge icon from material design.
    static const ImWchar icons_ranges[] = {ICON_MIN_MD, ICON_MAX_16_MD, 0};
    ImFontConfig icons_config;
    icons_config.MergeMode = true;
    icons_config.PixelSnapH = true;
    icons_config.GlyphOffset = ImVec2(0, 2.5);
    m_io->Fonts->AddFontFromFileTTF(materialIconPath.c_str(), 16.0f,
                                    &icons_config, icons_ranges);
  } else {
    logger->coreLogger->error("Icon loaded failed.");
  }
  // use FONT_ICON_FILE_NAME_FAR if you want regular instead of solid
}

WindowManager::~WindowManager() {
  // Cleanup
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  ImPlot::DestroyContext();

  // GLFW window cleanup.
  glfwTerminate();
}

int WindowManager::execute() {
  // Main loop.
  while (!glfwWindowShouldClose(m_window)) {
    // Event polling.
    glfwPollEvents();

    // Clear color and depth buffer.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // UI rendering here.

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    if (m_mainMenuBar) m_mainMenuBar->render();

    if (m_playControl) m_playControl->render();

    enableDockspace();

    if (m_mainStatusBar) m_mainStatusBar->render();

    // Render ImGuiObjects.
    for (int i = 0; i < m_renderObjs.size(); i++) {
      if (!m_renderObjs[i]->getEnabled()) continue;

      m_renderObjs[i]->render();
    }

    // Rendering
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Update and Render additional Platform Windows
    // (Platform functions may change the current OpenGL context, so we
    // save/restore it to make it easier to paste this code elsewhere.
    //  For this specific demo app we could also call
    //  glfwMakeContextCurrent(window) directly)
    if (m_io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
      GLFWwindow* backup_current_context = glfwGetCurrentContext();
      ImGui::UpdatePlatformWindows();
      ImGui::RenderPlatformWindowsDefault();
      glfwMakeContextCurrent(backup_current_context);
    }

    // Swap render buffer.
    glfwSwapBuffers(m_window);
  }

  hpaslt::logger->uiLogger->debug("GLFW window closed, exiting main loop.");
  return EXIT_SUCCESS;
}

void WindowManager::pushRenderObject(std::shared_ptr<ImGuiObject> renderObj) {
  m_renderObjs.push_back(renderObj);
}

}  // namespace hpaslt
