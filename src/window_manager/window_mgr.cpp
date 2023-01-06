#include "logger/logger.h"
#include "window_mgr.h"

namespace hpaslt {

WindowManager::WindowManager() {
  // Start the UI thread.
  hpaslt::logger->uiLogger->info("WindowManager created.");

  // Init GLFW.
  if (!glfwInit()) {
    hpaslt::logger->uiLogger->error("GLFW initialization failed!");
    throw std::runtime_error("GLFW initialization failed!");
  }
  hpaslt::logger->uiLogger->info("GLFW initialization success.");

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
  hpaslt::logger->uiLogger->info("GLFW window creation success.");

  glfwMakeContextCurrent(m_window);

  // Init GLEW.
  glewExperimental = true;
  if (glewInit() != GLEW_OK) {
    hpaslt::logger->uiLogger->error("GLEW initialization failed!");
    throw std::runtime_error("GLEW initialization failed!");
  }
  hpaslt::logger->uiLogger->info("GLEW initialization success.");
}

WindowManager::~WindowManager() {
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

    // TODO: UI rendering here.

    // Swap render buffer.
    glfwSwapBuffers(m_window);
  }

  hpaslt::logger->uiLogger->info("GLFW window closed, exiting main loop.");
  return EXIT_SUCCESS;
}

}  // namespace hpaslt
