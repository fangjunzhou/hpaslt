#include <memory>

#include <AudioFile.h>

namespace hpaslt {

class SignalGenerator {
 private:
  /**
   * @brief The current working audio file.
   *
   */
  std::shared_ptr<AudioFile<float>> m_workingAudioFile;

 public:
  /**
   * @brief Bind the current working audio file.
   *
   * @param audioFile
   */
  void bindAudioFile(std::shared_ptr<AudioFile<float>> audioFile) {
    m_workingAudioFile = audioFile;
  }

  /**
   * @brief Change the length of the audio file.
   *
   * @param length the new length of the audio file.
   */
  void changeLength(int length);

  /**
   * @brief Generate a base signal on the current audio file.
   *
   * @param freq the frequency of the signal.
   * @param magnitude the magnitude of the signal.
   */
  void generateSignal(float freq, float magnitude);

  /**
   * @brief Overlay a new signal on the exist signal.
   *
   * @param freq the frequency of the signal.
   * @param magnitude the magnitude of the signal.
   */
  void overlaySignal(float freq, float magnitude);
};

}  // namespace hpaslt
