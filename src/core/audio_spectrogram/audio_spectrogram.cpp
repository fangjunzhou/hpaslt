#include "audio_spectrogram.h"

namespace hpaslt {

SpectrogramFrame AudioSpectrogram::operator[](int indx) {
  return SpectrogramFrame(m_nfft, &m_rawSpectrogram[indx * m_nfft]);
}

void AudioSpectrogram::generateSpectrogram(std::shared_ptr<AudioObject> audiObj,
                                           int nfft) {
  // Set the sample rate and fft bin size.
  m_audioObj = audiObj;
  m_nfft = nfft;
  // TODO: Generate the spectrogram.
}

}  // namespace hpaslt
