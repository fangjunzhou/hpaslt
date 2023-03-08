#include <gtest/gtest.h>

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <math.h>
#include <AudioFile.h>

#include "core/signal_generator/signal_generator.h"

namespace hpaslt {

namespace test {

struct FreqComponent {
  float freq;
  float magnitude;
};

class SignalGeneratorTest : public ::testing::Test {
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

  SignalGeneratorTest() {}
  ~SignalGeneratorTest() override {}

  void SetUp() override {
    // Setup audio.
    m_audioFile = std::make_shared<AudioFile<float>>();
    m_audioFile->setNumChannels(2);

    // Setup signal generator.
    m_signalGenerator = std::make_shared<hpaslt::SignalGenerator>();
    m_signalGenerator->bindAudioFile(m_audioFile);
  }

  virtual void TearDown() override {
    m_audioFile = nullptr;
    m_signalGenerator = nullptr;
  }

  /**
   * @brief Check if the audio file signal is the combination of a list of
   * frequency.
   *
   * @param freq the frequency list.
   * @return testing::AssertionResult
   */
  testing::AssertionResult isFrequency(const std::vector<FreqComponent>& freq,
                                       float error = 0) {
    int sampleRate = m_audioFile->getSampleRate();
    for (int i = 0; i < m_audioFile->getNumSamplesPerChannel(); i++) {
      // Merge all the frequency together.
      float expectVal = 0;
      for (int j = 0; j < freq.size(); j++) {
        expectVal +=
            sin(2 * M_PI * i * freq[j].freq / sampleRate) * freq[j].magnitude;
      }

      for (int channel = 0; channel < m_audioFile->getNumChannels();
           channel++) {
        if (abs(m_audioFile->samples[channel][i] - expectVal) > error) {
          return testing::AssertionFailure()
                 << "Sample " << i << " at channel " << channel << " should be "
                 << expectVal << ", but is actually "
                 << m_audioFile->samples[channel][i];
        }
      }
    }

    return testing::AssertionSuccess();
  }
};

#define TEST_AUDIO_LENGTH 4
TEST_F(SignalGeneratorTest, ChangeLength) {
  // Check the default audio channel number and sample rate.
  ASSERT_EQ(m_audioFile->getNumChannels(), 2);
  ASSERT_EQ(m_audioFile->getSampleRate(), 44100);

  // Check the default audio length is 0.
  ASSERT_EQ(m_audioFile->getNumSamplesPerChannel(), 0);

  // Change the length of the audio.
  m_signalGenerator->changeLength(m_audioFile->getSampleRate() *
                                  TEST_AUDIO_LENGTH);

  // Check the new audio length.
  EXPECT_EQ(m_audioFile->getNumSamplesPerChannel(), 44100 * TEST_AUDIO_LENGTH);
  EXPECT_EQ(m_audioFile->getLengthInSeconds(), TEST_AUDIO_LENGTH);
}

TEST_F(SignalGeneratorTest, GenerateSignal) {
  // Change the length of the audio.
  m_signalGenerator->changeLength(m_audioFile->getSampleRate() *
                                  TEST_AUDIO_LENGTH);

  // Check the new audio length.
  EXPECT_EQ(m_audioFile->getNumSamplesPerChannel(), 44100 * TEST_AUDIO_LENGTH);
  EXPECT_EQ(m_audioFile->getLengthInSeconds(), TEST_AUDIO_LENGTH);

  // Generate signal.
  m_signalGenerator->generateSignal(32, 1);

  // Test signal.
  std::vector<FreqComponent> freq{{32, 1}};
  EXPECT_TRUE(isFrequency(freq));
}

TEST_F(SignalGeneratorTest, OverlaySignal) {
  // Change the length of the audio.
  m_signalGenerator->changeLength(m_audioFile->getSampleRate() *
                                  TEST_AUDIO_LENGTH);

  // Check the new audio length.
  EXPECT_EQ(m_audioFile->getNumSamplesPerChannel(), 44100 * TEST_AUDIO_LENGTH);
  EXPECT_EQ(m_audioFile->getLengthInSeconds(), TEST_AUDIO_LENGTH);

  // Generate signal.
  m_signalGenerator->generateSignal(32, (float)1 / (float)3);

  // Overlay 2 layers of new signal.
  m_signalGenerator->overlaySignal(64, (float)1 / (float)3);
  m_signalGenerator->overlaySignal(128, (float)1 / (float)3);

  // Test signal.
  std::vector<FreqComponent> freq{{32, (float)1 / (float)3},
                                  {64, (float)1 / (float)3},
                                  {128, (float)1 / (float)3}};
  EXPECT_TRUE(isFrequency(freq));
}

}  // namespace test

}  // namespace hpaslt
