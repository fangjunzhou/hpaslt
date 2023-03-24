#include "audio_spectrogram.h"

namespace hpaslt {

RawSpectrogram::RawSpectrogram(int size) {
  // Allocate the fftw complex array.
  m_rawSpectrogram = (fftwf_complex *)fftwf_alloc_complex(size);
  m_spectrogramSize = size;
}

RawSpectrogram::~RawSpectrogram() {
  // Free the allocated fftw complex array.
  fftwf_free(m_rawSpectrogram);
}

void AudioSpectrogram::generateSpectrogram(
    std::shared_ptr<AudioFile<float>> audioFile, int nfft) {
  // Set the sample rate and fft bin size.
  m_audioFile = audioFile;
  m_nfft = nfft;
  m_spectrogramLength = m_audioFile->getNumSamplesPerChannel() / m_nfft;

  /* ---------------- Generate the spectrogram ---------------- */

  int channelNum = m_audioFile->getNumChannels();

  // Clear the old raw spectrogram for all channels.
  m_rawSpectrograms.clear();
  // Init raw spectrogram for all channels.
  for (int i = 0; i < channelNum; i++) {
    m_rawSpectrograms.push_back(
        std::make_unique<RawSpectrogram>(m_nfft * m_spectrogramLength));
  }

  // Read the samples into the input fftw complex array.
  std::vector<fftwf_complex *> inputs;
  inputs.resize(channelNum);
  for (int channel = 0; channel < channelNum; channel++) {
    // Alloc the input array.
    inputs[channel] =
        (fftwf_complex *)fftwf_alloc_complex(m_nfft * m_spectrogramLength);
    // Read the data in as real number.
#pragma omp parallel for
    for (int i = 0; i < m_nfft * m_spectrogramLength; i++) {
      inputs[channel][i][0] = m_audioFile->samples[channel][i];
      inputs[channel][i][1] = 0;
    }
  }

  // Convert the input data into the raw spectrogram.
  for (int channel = 0; channel < channelNum; channel++) {
    for (int frame = 0; frame < m_spectrogramLength; frame++) {
      // Offset the frame number to get the input and output fftw complex
      // pointer.
      fftwf_complex *in = inputs[channel] + m_nfft * frame;
      fftwf_complex *out =
          m_rawSpectrograms[channel]->getRawSpectrogram() + m_nfft * frame;

      // Start the fftw plan.
      fftwf_plan p =
          fftwf_plan_dft_1d(m_nfft, in, out, FFTW_FORWARD, FFTW_MEASURE);

      // Execute the plan.
      fftwf_execute(p);

      // Destroy the plan.
      fftwf_destroy_plan(p);
    }
  }

  // Free the input array.
  for (int channel = 0; channel < channelNum; channel++) {
    fftwf_free(inputs[channel]);
  }
}

} // namespace hpaslt
