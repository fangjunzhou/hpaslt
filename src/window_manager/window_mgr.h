#pragma once

#include <future>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace hpaslt {

class WindowManager {
 private:
  /**
   * @brief the main GLFW window.
   *
   */
  GLFWwindow *m_window;

 public:
  /**
   * @brief Construct a new Window Manager object.
   *
   */
  WindowManager();

  /**
   * @brief Destroy the Window Manager object.
   *
   */
  ~WindowManager();

  /**
   * @brief Execute the main loop.
   *
   * @return int the execution result.
   */
  int execute();
};

}  // namespace hpaslt
