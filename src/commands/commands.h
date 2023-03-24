#pragma once

#include <csys/csys.h>
#include <eventpp/callbacklist.h>

#include <map>

namespace hpaslt {

class Commands {
private:
  static std::shared_ptr<Commands> s_commands;

  std::shared_ptr<csys::System> m_system;

public:
  /**
   * @brief Get the Commands singleton.
   * If the singleton does not exist, create one on the heap.
   *
   * @return std::weak_ptr<Commands>
   */
  static std::weak_ptr<Commands> getSingleton() {
    if (!s_commands) {
      s_commands = std::make_shared<Commands>();
    }
    return s_commands;
  }

  /**
   * @brief Call this method to free the singleton.
   *
   */
  static void freeSingleton() { s_commands = nullptr; }

  /**
   * @brief Disable the default copy constructor for Commands.
   *
   */
  Commands(const Commands &) = delete;

  /**
   * @brief Construct a new Commands object
   *
   */
  Commands();

  /**
   * @brief Destroy the Commands object
   *
   */
  ~Commands();

  /**
   * @brief Get the csys::System object.
   *
   * @return std::weak_ptr<csys::System>
   */
  std::weak_ptr<csys::System> getSystem() { return m_system; }
};

} // namespace hpaslt
