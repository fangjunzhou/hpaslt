#include <math.h>

#include "signal_generator.h"

namespace hpaslt {

void SignalGenerator::changeLength(int length) {
  m_workingAudioFile->setNumSamplesPerChannel(length);
}

void SignalGenerator::generateSignal(float freq, float magnitude) {
  int sampleRate = m_workingAudioFile->getSampleRate();
  int sampleLength = m_workingAudioFile->getNumSamplesPerChannel();
  for (int i = 0; i < sampleLength; i++) {
    for (int channel = 0; channel < m_workingAudioFile->getNumChannels();
         channel++) {
      m_workingAudioFile->samples[channel][i] =
          sin(2 * M_PI * i * freq / sampleRate) * magnitude;
    }
  }
}

void SignalGenerator::overlaySignal(float freq, float magnitude) {
  int sampleRate = m_workingAudioFile->getSampleRate();
  int sampleLength = m_workingAudioFile->getNumSamplesPerChannel();
  for (int i = 0; i < sampleLength; i++) {
    for (int channel = 0; channel < m_workingAudioFile->getNumChannels();
         channel++) {
      m_workingAudioFile->samples[channel][i] +=
          sin(2 * M_PI * i * freq / sampleRate) * magnitude;
    }
  }
}

}  // namespace hpaslt
