#pragma once

#include <vector>
#include <future>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "imgui_object.h"

namespace hpaslt {

class WindowManager {
 private:
  /**
   * @brief the main GLFW window.
   *
   */
  GLFWwindow* m_window;

  /**
   * @brief ImGui IO pointer.
   *
   */
  ImGuiIO* m_io;

  /**
   * @brief All the ImGuiObjects to be rendered.
   *
   */
  std::vector<ImGuiObject> m_renderObjs;

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

extern std::shared_ptr<WindowManager> windowMgr;

}  // namespace hpaslt
