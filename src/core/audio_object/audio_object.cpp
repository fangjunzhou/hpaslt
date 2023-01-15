#include "audio_object.h"

namespace hpaslt {

void AudioObject::loadAudioFile(const std::string& filePath) {
  // Guard the audio file.
  m_mutex.lock();

  if (!m_audioFile.load(filePath)) {
    throw std::invalid_argument("Audio file at path cannot be loaded.");
  }

  // Reset cursor.
  setCursor(0);
  m_mutex.unlock();

  if (m_onChangePlayingTime) (*m_onChangePlayingTime)(getTime(), getLength());
}

float AudioObject::getLength() {
  // Guard the audio file.
  m_mutex.lock();

  int numSamples = m_audioFile.getNumSamplesPerChannel();
  int sampleRate = m_audioFile.getSampleRate();

  m_mutex.unlock();

  return (float)numSamples / (float)sampleRate;
}

float AudioObject::getTime() {
  // Guard the audio file.
  m_mutex.lock();

  int cursorFrame = m_cursor;
  int sampleRate = m_audioFile.getSampleRate();

  m_mutex.unlock();

  return (float)cursorFrame / (float)sampleRate;
}

}  // namespace hpaslt
