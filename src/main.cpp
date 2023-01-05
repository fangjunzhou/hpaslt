#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

auto consoleLogger = spdlog::stdout_color_mt("console");

int main(int argc, char const *argv[]) {
  consoleLogger->info("HPASLT Started.");

  // Init GLFW.
  if (!glfwInit()) {
    consoleLogger->error("GLFW Init Failed!");
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
    consoleLogger->error("GLFW Window Creation Failed!");
    glfwTerminate();
    return EXIT_FAILURE;
  }
  glfwMakeContextCurrent(window);

  // Init GLEW.
  glewExperimental = true;
  if (glewInit() != GLEW_OK) {
    consoleLogger->error("GLEW Init Failed!");
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
