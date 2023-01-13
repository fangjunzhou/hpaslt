#pragma once

#include <string>

#include <AudioFile.h>

namespace hpaslt {
class AudioObject {
 private:
  /**
   * @brief The current processing audio file.
   *
   */
  AudioFile<double> m_audioFile;

 public:
  /**
   * @brief Load the audio file from path.
   *
   * @param filePath
   */
  void loadAudioFile(const std::string& filePath);

  /**
   * @brief Get the internal AudioFile reference.
   *
   * @return AudioFile<float>&
   */
  AudioFile<double>& getAudioFile() { return m_audioFile; }

  /**
   * @brief Get the AudioFile length in seconds.
   *
   * @return double
   */
  double getLength();
};

}  // namespace hpaslt
