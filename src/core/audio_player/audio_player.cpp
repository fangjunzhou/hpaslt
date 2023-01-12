#include <portaudio.h>

#include "audio_player.h"

#include "logger/logger.h"

namespace hpaslt {

std::shared_ptr<AudioPlayer> AudioPlayer::s_audioPlayer = nullptr;

AudioPlayer::AudioPlayer() {
  // Initialize PortAudio.
  PaError err = Pa_Initialize();
  if (err != paNoError) {
    logger->coreLogger->error("PortAudio initialization failed: {}",
                              Pa_GetErrorText(err));
    return;
  }
  logger->coreLogger->debug("PortAudio initialized.");
}

AudioPlayer::~AudioPlayer() {
  // Terminate PortAudio.
  PaError err = Pa_Terminate();
  if (err != paNoError) {
    logger->coreLogger->error("PortAudio termination failed: {}",
                              Pa_GetErrorText(err));
    return;
  }
  logger->coreLogger->debug("PortAudio terminated.");
}

}  // namespace hpaslt
