#pragma once

#include <unordered_map>
#include <memory>

#include <AudioFile.h>

#include "core/audio_object/audio_object.h"

namespace hpaslt {

/* ---------------------------------------------------------- */
/*                    Audio Workspace Class                   */
/* ---------------------------------------------------------- */

class AudioWorkspace {
 private:
  static std::unordered_map<std::string, std::shared_ptr<AudioWorkspace>>
      s_audioWorkspacecs;

  static std::shared_ptr<AudioWorkspace> s_currAudioWorkspace;

  /**
   * @brief The name of current audio workspace.
   *
   */
  std::string m_workspaceName;

  /**
   * @brief The current processing audio object.
   *
   */
  std::shared_ptr<AudioObject> m_audioObject;

 public:
  /**
   * @brief Get the current active AudioWorkspace.
   *
   * @return std::weak_ptr<AudioWorkspace>
   */
  static std::weak_ptr<AudioWorkspace> getCurrAudioWorkspace();

  /**
   * @brief Create a new AudioWorkspace.
   *
   * @param workspaceName
   */
  static void createAudioWorkspace(const std::string& workspaceName);

  /**
   * @brief Change the current AudioWorkspace.
   *
   * @param workspaceName
   */
  static void changeAudioWorkspace(const std::string& workspaceName);

  /**
   * @brief Register all the commands related to AudioWorkspace.
   *
   */
  static void registerConosleCommands();

  /**
   * @brief Construct a new AudioWorkspace object.
   *
   */
  AudioWorkspace(const std::string& workspaceName);
  /**
   * @brief Destroy the AudioWorkspace object.
   *
   */
  ~AudioWorkspace();

  /**
   * @brief Load a .wav audio file into m_audioFile.
   *
   * @param filePath
   */
  void loadAudioFile(const std::string& filePath);
};

}  // namespace hpaslt
