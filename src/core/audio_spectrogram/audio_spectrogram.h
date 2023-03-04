#pragma once

#include <vector>
#include <fftw3.h>

#include "core/audio_object/audio_object.h"

namespace hpaslt {

class RawSpectrogram {
 private:
  /**
   * @brief the raw spectrogram of the fft conversion.
   * The size of this vector should be m_nfft * m_spectrogramLength.
   *
   */
  fftwf_complex *m_rawSpectrogram;

  /**
   * @brief The size of the spectrogram.
   * The size should be m_nfft * m_spectrogramLength.
   *
   */
  int m_spectrogramSize;

 public:
  /**
   * @brief Get the raw spectrogram array pointer.
   *
   * @return fftwf_complex*
   */
  fftwf_complex *getRawSpectrogram() { return m_rawSpectrogram; }

  /**
   * @brief Get the size of the raw spectrogram array.
   *
   * @return int
   */
  int getSpectrogramSize() { return m_spectrogramSize; }

  /**
   * @brief Construct a new RawSpectrogram object.
   *
   * @param size The size of the raw spectrogram array.
   */
  RawSpectrogram(int size);

  /**
   * @brief Destroy the RawSpectrogram object.
   *
   */
  ~RawSpectrogram();
};

class AudioSpectrogram {
 private:
  /**
   * @brief The audio object shared pointer.
   *
   */
  std::shared_ptr<AudioObject> m_audioObj;

  /**
   * @brief The fft bin size.
   *
   */
  int m_nfft;

  /**
   * @brief The length of the spectrogram.
   *
   */
  int m_spectrogramLength;

  /**
   * @brief the raw spectrogram of all channels.
   *
   */
  std::vector<RawSpectrogram> m_rawSpectrograms;

 public:
  /**
   * @brief Get the fft bin size of the spectrogram.
   *
   * @return int
   */
  int getNfft() { return m_nfft; }

  /**
   * @brief Get the original audio object sample rate.
   *
   * @return int
   */
  int getAudioSampleRate() {
    return m_audioObj->getAudioFile().getSampleRate();
  }

  /**
   * @brief Get the original audio object sample frequency.
   * This is frequency corresponding to the last bin in each spectrogram frame.
   *
   * @return float
   */
  float getAudioSampleFreq() { return 1 / (float)getAudioSampleRate(); }

  /**
   * @brief Get the sample rate of the spectrogram.
   * The spectrogram sample rate should be original audio sample rate / nfft.
   *
   * @return int
   */
  float getSpectrogramSampleRate() {
    return (float)getAudioSampleRate() / (float)m_nfft;
  }

  /**
   * @brief Get the reference to the raw spectrogram reference.
   *
   * @return std::vector<RawSpectrogram>&
   */
  std::vector<RawSpectrogram> &getRawSpectrogram() { return m_rawSpectrograms; }

  /**
   * @brief Construct a new AudioSpectrogram object.
   *
   */
  AudioSpectrogram() {}

  /**
   * @brief Destroy the AudioSpectrogram object.
   *
   */
  ~AudioSpectrogram() {}

  /**
   * @brief Generate a new spectrogram with the audio object and fft bin size.
   *
   * @param audioObj
   * @param nfft
   */
  void generateSpectrogram(std::shared_ptr<AudioObject> audioObj, int nfft);

  // TODO: Use IFFT to get the original audio from the spectrogram.
};

}  // namespace hpaslt
