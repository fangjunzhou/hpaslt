#include "audio_player.h"

#include "logger/logger.h"
#include "core/audio_object/audio_object.h"

// TODO: Add settings for frame per buffer.
#define FRAME_PER_BUFFER 4096

namespace hpaslt {

int AudioPlayer::paCallback(const void *inputBuffer, void *outputBuffer,
                            unsigned long framesPerBuffer,
                            const PaStreamCallbackTimeInfo *timeInfo,
                            PaStreamCallbackFlags statusFlags, void *userData) {
  AudioPlayer *audioPlayer = (AudioPlayer *)userData;
  AudioObject *audioObj = audioPlayer->m_audioObj.get();
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
      // Set cursor.
      audioObj->setCursor(0);
      audioObj->getMutex().unlock();
      // Invoke time callback.
      auto timeCallback = audioObj->getTimeCallback();
      if (timeCallback)
        (*timeCallback)(audioObj->getTime(), audioObj->getLength());
      // Stop the stream.
      audioPlayer->m_isPlaying = false;
      audioPlayer->m_onChangePlayingStatus(false);
      audioPlayer->m_needStopBeforeStartStream = true;
      logger->coreLogger->trace("AudioPlayer finished.");
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

AudioPlayer::AudioPlayer()
    : m_stream(nullptr),
      m_isPlaying(false),
      m_needStopBeforeStartStream(false) {}

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
  // Pause current stream.
  pause();

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
                      FRAME_PER_BUFFER, paClipOff, paCallback, this);
  m_audioObj->getMutex().unlock();

  if (err != paNoError) {
    portAudioError(err);
    return;
  }

  // Reset m_needStopBeforeStartStream.
  m_needStopBeforeStartStream = false;

  logger->coreLogger->trace("New pa stream created.");
}

void AudioPlayer::play() {
  if (!m_audioObj) return;

  if (m_isPlaying) {
    logger->coreLogger->trace("AudioPlayer already playing.");
    return;
  }

  if (m_needStopBeforeStartStream) {
    PaError err = Pa_StopStream(m_stream);
    if (err != paNoError) {
      portAudioError(err);
      return;
    }
    m_needStopBeforeStartStream = false;
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
  if (!m_audioObj) return;

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
  m_needStopBeforeStartStream = false;

  logger->coreLogger->trace("AudioPlayer paused.");
}

void AudioPlayer::replay() {
  if (!m_audioObj) return;

  m_audioObj->setCursor(0);
  auto timeCallback = m_audioObj->getTimeCallback();
  if (timeCallback)
    (*timeCallback)(m_audioObj->getTime(), m_audioObj->getLength());
  play();
}

void AudioPlayer::stop() {
  if (!m_audioObj) return;

  pause();
  m_audioObj->setCursor(0);
  auto timeCallback = m_audioObj->getTimeCallback();
  if (timeCallback)
    (*timeCallback)(m_audioObj->getTime(), m_audioObj->getLength());
}

void AudioPlayer::setTime(float time) {
  if (!m_audioObj) return;

  m_audioObj->getMutex().lock();

  // Get the audio file.
  AudioFile<float> &audioFile = m_audioObj->getAudioFile();
  // Get the new cursor frame.
  int cursorFrame = (int)(audioFile.getSampleRate() * time);

  // Bound the frame.
  int maxFrame = audioFile.getNumSamplesPerChannel();
  if (cursorFrame < 0) {
    cursorFrame = 0;
  } else if (cursorFrame > maxFrame) {
    cursorFrame = maxFrame;
  }

  m_audioObj->setCursor(cursorFrame);

  m_audioObj->getMutex().unlock();
}

}  // namespace hpaslt
