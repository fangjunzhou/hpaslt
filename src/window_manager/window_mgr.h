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
  static std::shared_ptr<WindowManager> s_windowMgr;

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
  std::vector<std::shared_ptr<ImGuiObject>> m_renderObjs;

 public:
  /**
   * @brief Get the WindowManager singleton.
   * If the singleton does not exist, create one on the heap.
   *
   * @return WindowManager*
   */
  static std::weak_ptr<WindowManager> getSingleton() {
    if (!s_windowMgr) {
      s_windowMgr = std::make_shared<WindowManager>();
    }
    return s_windowMgr;
  }

  /**
   * @brief Call this method to free the singleton.
   *
   * After this call, all the objects using WindowManager
   * singleton should expired.
   *
   */
  static void freeSingleton() { s_windowMgr = nullptr; }

  /**
   * @brief Disable the copy constructor for WindowManager.
   *
   */
  WindowManager(const WindowManager&) = delete;

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

  /**
   * @brief Push a new render object to the vector.
   *
   * @param renderObj the reference to the new object.
   */
  void pushRenderObject(std::shared_ptr<ImGuiObject> renderObj);
};

}  // namespace hpaslt
