#pragma once

#include <memory>

namespace hpaslt {

class AudioPlayer {
 private:
  static std::shared_ptr<AudioPlayer> s_audioPlayer;

 public:
  /**
   * @brief Get the AudioPlayer singleton.
   *
   * @return std::weak_ptr<AudioPlayer>
   */
  static std::weak_ptr<AudioPlayer> getSingleton() {
    if (!s_audioPlayer) {
      s_audioPlayer = std::make_shared<AudioPlayer>();
    }

    return s_audioPlayer;
  }

  /**
   * @brief Free the AudioPlayer singleton.
   *
   */
  static void freeSingleton() { s_audioPlayer = nullptr; }

  /**
   * @brief Construct a new AudioPlayer object.
   *
   */
  AudioPlayer();

  /**
   * @brief Destroy the AudioPlayer object.
   *
   */
  ~AudioPlayer();
};

}  // namespace hpaslt
