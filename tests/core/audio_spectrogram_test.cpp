#include <gtest/gtest.h>

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <math.h>
#include <algorithm>
#include <AudioFile.h>
#include <limits>

#include "core/signal_generator/signal_generator.h"
#include "core/audio_spectrogram/audio_spectrogram.h"

namespace hpaslt {

namespace test {

class AudioSpectrogramTest : public ::testing::Test {
 protected:
  /**
   * @brief The audio file object used by all tests.
   *
   */
  std::shared_ptr<AudioFile<float>> m_audioFile;

  /**
   * @brief The signal generator used by all tests.
   *
   */
  std::shared_ptr<hpaslt::SignalGenerator> m_signalGenerator;

  /**
   * @brief The spectrogram object to generate and store the spectrogram.
   *
   */
  std::shared_ptr<hpaslt::AudioSpectrogram> m_audioSpectrogram;

  AudioSpectrogramTest() {}
  ~AudioSpectrogramTest() override {}

  void SetUp() override {
    // Setup audio.
    m_audioFile = std::make_shared<AudioFile<float>>();
    m_audioFile->setNumChannels(2);

    // Setup signal generator.
    m_signalGenerator = std::make_shared<hpaslt::SignalGenerator>();
    m_signalGenerator->bindAudioFile(m_audioFile);

    m_audioSpectrogram = std::make_shared<hpaslt::AudioSpectrogram>();
  }

  virtual void TearDown() override {
    m_audioFile = nullptr;
    m_signalGenerator = nullptr;
    m_audioSpectrogram = nullptr;
  }

  /**
   * @brief Check if the freq list contains the target frequency within err.
   *
   * @param freq a vector of frequencies.
   * @param target the target frequency.
   * @param err allowed error.
   * @return testing::AssertionResult
   */
  testing::AssertionResult containsFreq(const std::vector<float>& freq,
                                        float target, float err) {
    float minDiffFreq = 0;
    float minDiff = std::numeric_limits<float>::max();

    for (int i = 0; i < freq.size(); i++) {
      if (std::abs(freq[i] - target) <= err) {
        return testing::AssertionSuccess();
      }
      if (std::abs(freq[i] - target) <= minDiff) {
        minDiff = std::abs(freq[i] - target);
        minDiffFreq = freq[i];
      }
    }

    return testing::AssertionFailure()
           << "Minimum frequency error between " << target << " and "
           << minDiffFreq << " is " << minDiff
           << ". This is larger than allowed error " << err;
  }
};

#define TEST_AUDIO_LENGTH 2
TEST_F(AudioSpectrogramTest, GenerateSpectrogramNFFT) {
  // Change the length of the audio.
  m_signalGenerator->changeLength(m_audioFile->getSampleRate() *
                                  TEST_AUDIO_LENGTH);

  // Check the new audio length.
  ASSERT_EQ(m_audioFile->getNumSamplesPerChannel(), 44100 * TEST_AUDIO_LENGTH);
  ASSERT_EQ(m_audioFile->getLengthInSeconds(), TEST_AUDIO_LENGTH);

  // Generate signal.
  m_signalGenerator->generateSignal(32, (float)1 / (float)3);

  // Overlay 2 layers of new signal.
  m_signalGenerator->overlaySignal(64, (float)1 / (float)3);
  m_signalGenerator->overlaySignal(128, (float)1 / (float)3);

  // Convert the audio to spectrogram.
  m_audioSpectrogram->generateSpectrogram(m_audioFile, 128);
  EXPECT_EQ(m_audioSpectrogram->getNfft(), 128);

  m_audioSpectrogram->generateSpectrogram(m_audioFile, 256);
  EXPECT_EQ(m_audioSpectrogram->getNfft(), 256);

  m_audioSpectrogram->generateSpectrogram(m_audioFile, 512);
  EXPECT_EQ(m_audioSpectrogram->getNfft(), 512);
}

TEST_F(AudioSpectrogramTest, GenerateSpectrogramSampleRate) {
  // Change the length of the audio.
  m_signalGenerator->changeLength(m_audioFile->getSampleRate() *
                                  TEST_AUDIO_LENGTH);

  // Check the new audio length.
  ASSERT_EQ(m_audioFile->getNumSamplesPerChannel(), 44100 * TEST_AUDIO_LENGTH);
  ASSERT_EQ(m_audioFile->getLengthInSeconds(), TEST_AUDIO_LENGTH);

  // Generate signal.
  m_signalGenerator->generateSignal(32, (float)1 / (float)3);

  // Overlay 2 layers of new signal.
  m_signalGenerator->overlaySignal(64, (float)1 / (float)3);
  m_signalGenerator->overlaySignal(128, (float)1 / (float)3);

  // Convert the audio to spectrogram.
  m_audioSpectrogram->generateSpectrogram(m_audioFile, 128);
  EXPECT_EQ(m_audioSpectrogram->getAudioSampleRate(),
            m_audioFile->getSampleRate());
}

#define NFFT 512
#define MAX_ERR 32

TEST_F(AudioSpectrogramTest, GenerateSpectrogramPeakFreq) {
  // Change the length of the audio.
  m_signalGenerator->changeLength(m_audioFile->getSampleRate() *
                                  TEST_AUDIO_LENGTH);

  // Check the new audio length.
  ASSERT_EQ(m_audioFile->getNumSamplesPerChannel(), 44100 * TEST_AUDIO_LENGTH);
  ASSERT_EQ(m_audioFile->getLengthInSeconds(), TEST_AUDIO_LENGTH);

  const std::vector<float> freqs{512, 1024, 2048};

  // Generate signal.
  m_signalGenerator->generateSignal(freqs[0], (float)1 / (float)freqs.size());

  // Overlay 2 layers of new signal.
  for (int i = 1; i < freqs.size(); i++) {
    m_signalGenerator->overlaySignal(freqs[i], (float)1 / (float)freqs.size());
  }

  // Convert the audio to spectrogram.
  m_audioSpectrogram->generateSpectrogram(m_audioFile, NFFT);

  auto& rawSpectrogram = m_audioSpectrogram->getRawSpectrogram();
  for (int ch = 0; ch < rawSpectrogram.size(); ch++) {
    auto& channel = rawSpectrogram[ch];
    // Get the middle frame.
    fftwf_complex* frame =
        channel->getRawSpectrogram() +
        (m_audioSpectrogram->getSpectrogramLength() / 2) * NFFT;
    // Get all the frequency magnitude.
    std::vector<std::pair<float, int>> freqMag;
    freqMag.resize(NFFT / 2);
    for (int i = 0; i < NFFT / 2; i++) {
      float mag = sqrt(pow(frame[i][0], 2) + pow(frame[i][1], 2));
      freqMag[i] = std::make_pair(mag, i);
    }
    // Get the index of the top 3 frequencies.
    std::sort(
        freqMag.begin(), freqMag.end(),
        [](const std::pair<float, int>& a, const std::pair<float, int>& b) {
          return (a.first > b.first);
        });

    for (int i = 0; i < 3; i++) {
      float freq =
          ((double)freqMag[i].second / (double)m_audioSpectrogram->getNfft()) *
          (double)m_audioSpectrogram->getAudioSampleRate();
      // Check if the frequency is in the generated frequency list.
      EXPECT_TRUE(containsFreq(freqs, freq, MAX_ERR));
    }
  }
}

}  // namespace test

}  // namespace hpaslt
