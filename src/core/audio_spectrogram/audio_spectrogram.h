#pragma once

#include <vector>
#include <fftw3.h>

#include "core/audio_object/audio_object.h"

namespace hpaslt {

/**
 * @brief A frame of audio spectrogram.
 *
 */
class SpectrogramFrame {
 private:
  /**
   * @brief The fft bin size.
   * This is also the size of the frame size.
   *
   */
  int m_nfft;

  /**
   * @brief The ptr pointing to the start of the frame.
   *
   */
  fftw_complex* m_frame;

 public:
  /**
   * @brief Construct a new SpectrogramFrame object.
   *
   * @param nfft
   * @param framePtr
   */
  SpectrogramFrame(int nfft, fftw_complex* framePtr) {
    m_nfft = nfft;
    m_frame = framePtr;
  }

  fftw_complex& operator[](int indx) {
    if (indx >= m_nfft || indx < 0) {
      throw std::invalid_argument("Frame index out of range.");
    }
    return m_frame[indx];
  }
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
   * @brief the raw spectrogram of the fft conversion.
   *
   */
  std::vector<fftw_complex> m_rawSpectrogram;

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

  /**
   * @brief operator overload for []. Get a frame.
   *
   * @param indx
   * @return SpectrogramFrame
   */
  SpectrogramFrame operator[](int indx);
};

}  // namespace hpaslt
