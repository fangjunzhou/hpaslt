#include "audio_player.h"

#include "logger/logger.h"
#include "core/audio_object/audio_object.h"

namespace hpaslt {

int AudioPlayer::paCallback(const void *inputBuffer, void *outputBuffer,
                            unsigned long framesPerBuffer,
                            const PaStreamCallbackTimeInfo *timeInfo,
                            PaStreamCallbackFlags statusFlags, void *userData) {
  AudioObject *audioObj = (AudioObject *)userData;
  float *out = (float *)outputBuffer;

  // Prevent unused variable warnings.
  (void)timeInfo;
  (void)statusFlags;
  (void)inputBuffer;

  audioObj->getMutex().lock();

  int channelNum = audioObj->getAudioFile().getNumChannels();
  int sampleNum = audioObj->getAudioFile().getNumSamplesPerChannel();
  auto samples = audioObj->getAudioFile().samples;
  int cursor = audioObj->getCursor();

  // TODO: Replace the for loop with memcpy.
  for (unsigned long i = 0; i < framesPerBuffer; i++) {
    // Reach the end of the audio.
    if (cursor + i >= sampleNum) {
      audioObj->setCursor(sampleNum);
      return paComplete;
    }
    // Copy data.
    for (int channel = 0; channel < channelNum; channel++) {
      *out++ = samples[channel][cursor + i];
    }
  }

  // Update the cursor.
  audioObj->setCursor(cursor + framesPerBuffer);

  audioObj->getMutex().unlock();

  return paContinue;
}

AudioPlayer::AudioPlayer() : m_stream(nullptr) {}

AudioPlayer::~AudioPlayer() {
  // Check if need to clean up.
  if (m_stream) {
    PaError err = Pa_CloseStream(m_stream);
    if (err != paNoError) {
      portAudioError(err);
      return;
    }
    logger->coreLogger->trace("AudioPlayer destructed, pa stream cleaned up.");
  }
}

void AudioPlayer::loadAudioObject(std::weak_ptr<AudioObject> audioObj) {
  // Load the object.
  m_audioObj = audioObj.lock();

  // Create the stream.
  PaStreamParameters outputParameters;
  PaError err;

  outputParameters.device = Pa_GetDefaultOutputDevice();
  if (outputParameters.device == paNoDevice) {
    logger->coreLogger->error("No default output device.");
    return;
  }
  m_audioObj->getMutex().lock();
  outputParameters.channelCount = m_audioObj->getAudioFile().getNumChannels();
  m_audioObj->getMutex().unlock();
  outputParameters.sampleFormat = paFloat32;
  outputParameters.suggestedLatency =
      Pa_GetDeviceInfo(outputParameters.device)->defaultLowOutputLatency;
  outputParameters.hostApiSpecificStreamInfo = NULL;

  // Check if need to close old stream.
  if (m_stream) {
    err = Pa_CloseStream(m_stream);
    if (err != paNoError) {
      portAudioError(err);
      return;
    }
    logger->coreLogger->trace("Old pa stream cleaned up.");
  }

  // Open new stream.
  m_audioObj->getMutex().lock();
  err = Pa_OpenStream(&m_stream, nullptr, &outputParameters,
                      m_audioObj->getAudioFile().getSampleRate(),
                      paFramesPerBufferUnspecified, paClipOff, paCallback,
                      m_audioObj.get());
  m_audioObj->getMutex().unlock();

  if (err != paNoError) {
    portAudioError(err);
    return;
  }

  logger->coreLogger->trace("New pa stream created.");

  // TODO: Set finish stream callback.
}

void AudioPlayer::play() {
  PaError err = Pa_StartStream(m_stream);
  if (err != paNoError) {
    portAudioError(err);
    return;
  }

  logger->coreLogger->debug("AudioPlayer played.");
}

void AudioPlayer::pause() {
  PaError err = Pa_StopStream(m_stream);
  if (err != paNoError) {
    portAudioError(err);
    return;
  }

  logger->coreLogger->debug("AudioPlayer paused.");
}

}  // namespace hpaslt
