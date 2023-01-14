#pragma once

#include <string>
#include <mutex>

#include <AudioFile.h>

namespace hpaslt {

class AudioObject {
  /**
   * @brief The lock to protect audio object.
   * Need to acquire the lock to access the audio object.
   *
   */
  std::mutex m_mutex;

 private:
  /**
   * @brief The current processing audio file.
   *
   */
  AudioFile<float> m_audioFile;

  /**
   * @brief Current playing frame.
   *
   */
  int m_cursor;

 public:
  /**
   * @brief Load the audio file from path.
   * This method is thread safe.
   *
   * @param filePath
   */
  void loadAudioFile(const std::string& filePath);

  /**
   * @brief Get the internal AudioFile reference.
   * This method is not thread safe.
   *
   * @return AudioFile<float>&
   */
  AudioFile<float>& getAudioFile() { return m_audioFile; }

  /**
   * @brief Get the mutex lock by reference.
   * This method is not thread safe.
   *
   * @return std::mutex&
   */
  std::mutex& getMutex() { return m_mutex; }

  /**
   * @brief Get the AudioFile length in seconds.
   * This method is thread safe.
   *
   * @return double
   */
  double getLength();

  /**
   * @brief Get the current playing frame.
   * This method is not thread safe.
   *
   * @return int
   */
  int getCursor() { return m_cursor; }

  /**
   * @brief Set the current playing frame.
   *
   * @param cursor
   */
  void setCursor(int cursor) { m_cursor = cursor; }
};

}  // namespace hpaslt
