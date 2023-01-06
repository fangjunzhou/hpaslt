#include <memory>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "logger/logger.h"

int main(int argc, char const *argv[]) {
  std::unique_ptr<hpaslt::Logger> logger =
      std::make_unique<hpaslt::Logger>("log");

  logger->coreLogger->info("HPASLT Started.");

  // Init GLFW.
  if (!glfwInit()) {
    logger->uiLogger->error("GLFW Init Failed!");
    return EXIT_FAILURE;
  }

  // Enable GLFW 4x MSAA.
  glfwWindowHint(GLFW_SAMPLES, 4);
  // Set OpenGL version.
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  // Mac compatability.
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window = glfwCreateWindow(960, 540, "HPASLT", nullptr, nullptr);
  if (!window) {
    logger->uiLogger->error("GLFW Window Creation Failed!");
    glfwTerminate();
    return EXIT_FAILURE;
  }
  glfwMakeContextCurrent(window);

  // Init GLEW.
  glewExperimental = true;
  if (glewInit() != GLEW_OK) {
    logger->uiLogger->error("GLEW Init Failed!");
    glfwTerminate();
    return EXIT_FAILURE;
  }

  // Main loop.
  while (!glfwWindowShouldClose(window)) {
    // Event polling.
    glfwPollEvents();

    // Clear color and depth buffer.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Swap render buffer.
    glfwSwapBuffers(window);
  }

  return EXIT_SUCCESS;
}
