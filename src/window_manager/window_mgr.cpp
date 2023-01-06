#include "logger/logger.h"
#include "window_mgr.h"

namespace hpaslt {

std::shared_ptr<WindowManager> windowMgr = nullptr;

WindowManager::WindowManager() {
  hpaslt::logger->uiLogger->debug("WindowManager created.");

  // Init GLFW.
  if (!glfwInit()) {
    hpaslt::logger->uiLogger->error("GLFW initialization failed!");
    throw std::runtime_error("GLFW initialization failed!");
  }
  hpaslt::logger->uiLogger->debug("GLFW initialization success.");

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
  hpaslt::logger->uiLogger->debug("GLFW window creation success.");

  glfwMakeContextCurrent(m_window);

  // Init GLEW.
  glewExperimental = true;
  if (glewInit() != GLEW_OK) {
    hpaslt::logger->uiLogger->error("GLEW initialization failed!");
    throw std::runtime_error("GLEW initialization failed!");
  }
  hpaslt::logger->uiLogger->debug("GLEW initialization success.");

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
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

  // TODO: Load fonts here.
}

WindowManager::~WindowManager() {
  // GLFW window cleanup.
  glfwTerminate();
}

int WindowManager::execute() {
  bool showDemoWindow;
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

    // TODO: Remove the demo window here.
    // Show demo window.
    ImGui::ShowDemoWindow(&showDemoWindow);

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
