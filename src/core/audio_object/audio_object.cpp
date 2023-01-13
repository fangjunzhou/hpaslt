#include "audio_object.h"

namespace hpaslt {

void AudioObject::loadAudioFile(const std::string& filePath) {
  // Guard the audio file.
  std::lock_guard<std::mutex> lock(m_mutex);

  if (!m_audioFile.load(filePath)) {
    throw std::invalid_argument("Audio file at path cannot be loaded.");
  }

  resetCursor();
}

double AudioObject::getLength() {
  // Guard the audio file.
  m_mutex.lock();

  int numSamples = m_audioFile.getNumSamplesPerChannel();
  int sampleRate = m_audioFile.getSampleRate();

  m_mutex.unlock();

  return (double)numSamples / (double)sampleRate;
}

}  // namespace hpaslt
