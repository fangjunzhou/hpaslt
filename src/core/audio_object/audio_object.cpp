#include "audio_object.h"

namespace hpaslt {

void AudioObject::loadAudioFile(const std::string& filePath) {
  std::shared_ptr<AudioFile<float>> targetFile =
      std::make_shared<AudioFile<float>>();
  if (!targetFile->load(filePath)) {
    throw std::invalid_argument("Audio file at path cannot be loaded.");
  }
  if (targetFile->getNumChannels() > 2) {
    throw std::invalid_argument("Audio file channel number not supported.");
  }

  // Guard the audio file.
  m_mutex.lock();
  // Set audio file.
  m_audioFile = targetFile;
  // Reset cursor.
  setCursor(0);
  m_mutex.unlock();

  if (m_onChangePlayingTime) (*m_onChangePlayingTime)(getTime(), getLength());
}

float AudioObject::getLength() {
  // Guard the audio file.
  m_mutex.lock();

  int numSamples = m_audioFile->getNumSamplesPerChannel();
  int sampleRate = m_audioFile->getSampleRate();

  m_mutex.unlock();

  return (float)numSamples / (float)sampleRate;
}

float AudioObject::getTime() {
  // Guard the audio file.
  m_mutex.lock();

  int cursorFrame = m_cursor;
  int sampleRate = m_audioFile->getSampleRate();

  m_mutex.unlock();

  return (float)cursorFrame / (float)sampleRate;
}

}  // namespace hpaslt
