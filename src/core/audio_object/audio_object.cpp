#include "audio_object.h"

namespace hpaslt {

void AudioObject::loadAudioFile(const std::string& filePath) {
  if (!m_audioFile.load(filePath)) {
    throw std::invalid_argument("Audio file at path cannot be loaded.");
  }
}

double AudioObject::getLength() {
  int numSamples = m_audioFile.getNumSamplesPerChannel();
  int sampleRate = m_audioFile.getSampleRate();

  return (double)numSamples / (double)sampleRate;
}

}  // namespace hpaslt
