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
      audioObj->getMutex().unlock();

      // Invoke time callback.
      auto timeCallback = audioObj->getTimeCallback();
      if (timeCallback)
        (*timeCallback)(audioObj->getTime(), audioObj->getLength());
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

  // Invoke time callback.
  auto timeCallback = audioObj->getTimeCallback();
  if (timeCallback) (*timeCallback)(audioObj->getTime(), audioObj->getLength());

  return paContinue;
}

AudioPlayer::AudioPlayer() : m_stream(nullptr), m_isPlaying(false) {}

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
  // Reset previous AudioObject callback.
  if (m_audioObj) {
    m_audioObj->resetTimeCallback();
  }

  // Load the object.
  m_audioObj = audioObj.lock();

  // Set AudioObject callback.
  m_audioObj->setTimeCallback(&m_onChangePlayingTime);
  // Initialize callback.
  m_onChangePlayingTime(m_audioObj->getTime(), m_audioObj->getLength());

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
  if (m_isPlaying) {
    logger->coreLogger->trace("AudioPlayer already playing.");
    return;
  }

  PaError err = Pa_StartStream(m_stream);
  if (err != paNoError) {
    portAudioError(err);
    return;
  }

  m_isPlaying = true;
  m_onChangePlayingStatus(true);

  logger->coreLogger->trace("AudioPlayer played.");
}

void AudioPlayer::pause() {
  if (!m_isPlaying) {
    logger->coreLogger->trace("AudioPlayer already paused.");
    return;
  }

  PaError err = Pa_StopStream(m_stream);
  if (err != paNoError) {
    portAudioError(err);
    return;
  }

  m_isPlaying = false;
  m_onChangePlayingStatus(false);

  logger->coreLogger->trace("AudioPlayer paused.");
}

void AudioPlayer::replay() {
  m_audioObj->setCursor(0);
  auto timeCallback = m_audioObj->getTimeCallback();
  if (timeCallback)
    (*timeCallback)(m_audioObj->getTime(), m_audioObj->getLength());
  play();
}

void AudioPlayer::stop() {
  pause();
  m_audioObj->setCursor(0);
  auto timeCallback = m_audioObj->getTimeCallback();
  if (timeCallback)
    (*timeCallback)(m_audioObj->getTime(), m_audioObj->getLength());
}

}  // namespace hpaslt
