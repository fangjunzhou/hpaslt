#pragma once

#include <unordered_map>
#include <memory>

#include <AudioFile.h>

#include "core/audio_object/audio_object.h"
#include "core/audio_player/audio_player.h"

namespace hpaslt {

/* ---------------------------------------------------------- */
/*                    Audio Workspace Class                   */
/* ---------------------------------------------------------- */

class AudioWorkspace {
 private:
  /**
   * @brief AudioWorkspace singleton.
   *
   */
  static std::shared_ptr<AudioWorkspace> s_audioWorkspace;

  /**
   * @brief Audio player for current workspace.
   *
   */
  std::shared_ptr<AudioPlayer> m_player;

  /**
   * @brief The current processing audio object.
   *
   */
  std::shared_ptr<AudioObject> m_audioObject;

 public:
  /**
   * @brief Get the AudioWorkspace singleton.
   *
   * @return std::weak_ptr<AudioWorkspace>
   */
  static std::weak_ptr<AudioWorkspace> getSingleton() {
    if (!s_audioWorkspace) {
      s_audioWorkspace = std::make_shared<AudioWorkspace>();
    }

    return s_audioWorkspace;
  }

  /**
   * @brief Free the AudioWorkspace singleton.
   *
   */
  static void freeSingleton() { s_audioWorkspace = nullptr; }

  /**
   * @brief Register all the commands related to AudioWorkspace.
   *
   */
  static void registerConosleCommands();

  /**
   * @brief Construct a new AudioWorkspace object.
   *
   */
  AudioWorkspace();
  /**
   * @brief Destroy the AudioWorkspace object.
   *
   */
  ~AudioWorkspace();

  /**
   * @brief Get the AudioPlayer.
   *
   * @return std::weak_ptr<AudioPlayer>
   */
  std::weak_ptr<AudioPlayer> getAudioPlayer() { return m_player; }

  /**
   * @brief Load a .wav audio file into m_audioFile.
   *
   * @param filePath
   */
  void loadAudioFile(const std::string& filePath);
};

}  // namespace hpaslt
