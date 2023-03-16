#include "audio_workspace.h"

#include <thread>

#include "commands/commands.h"
#include "logger/logger.h"

namespace hpaslt {

std::shared_ptr<AudioWorkspace> AudioWorkspace::s_audioWorkspace = nullptr;

eventpp::CallbackList<void(std::weak_ptr<AudioObject>)>
    AudioWorkspace::s_onAudioLoaded;

void AudioWorkspace::registerConosleCommands() {
  // Get the console system.
  std::shared_ptr<csys::System> system =
      Commands::getSingleton().lock()->getSystem().lock();

  system->RegisterCommand(
      "loadAudioCurr", "Load audio to current workspace.",
      [](const csys::String& audioFilePath) {
        std::shared_ptr<AudioWorkspace> currWorkspace =
            AudioWorkspace::getSingleton().lock();

        currWorkspace->loadAudioFile(audioFilePath.m_String);
      },
      csys::Arg<csys::String>("path"));

  system->RegisterCommand("playAudioCurr",
                          "Play the audio of current workspace.", []() {
                            std::shared_ptr<AudioWorkspace> currWorkspace =
                                AudioWorkspace::getSingleton().lock();

                            currWorkspace->m_player->play();
                          });

  system->RegisterCommand("pauseAudioCurr",
                          "Pause the audio of current workspace.", []() {
                            std::shared_ptr<AudioWorkspace> currWorkspace =
                                AudioWorkspace::getSingleton().lock();

                            currWorkspace->m_player->pause();
                          });

  system->RegisterCommand("replayAudioCurr",
                          "Replay the audio of current workspace.", []() {
                            std::shared_ptr<AudioWorkspace> currWorkspace =
                                AudioWorkspace::getSingleton().lock();

                            currWorkspace->m_player->replay();
                          });

  system->RegisterCommand("stopAudioCurr",
                          "Stop the audio of current workspace.", []() {
                            std::shared_ptr<AudioWorkspace> currWorkspace =
                                AudioWorkspace::getSingleton().lock();

                            currWorkspace->m_player->stop();
                          });

  logger->coreLogger->debug("AudioWorkspace commands registered.");
}

AudioWorkspace::AudioWorkspace() {
  // Alloc members.
  m_player = std::make_shared<AudioPlayer>();
  m_audioObject = std::make_shared<AudioObject>();
}

AudioWorkspace::~AudioWorkspace() {
  m_player = nullptr;
  m_audioObject = nullptr;
}

void AudioWorkspace::loadAudioFile(const std::string& filePath) {
  // Launch the loading thread.
  std::thread loadThread([this, filePath]() {
    try {
      m_audioObject->loadAudioFile(filePath);
    } catch (const std::invalid_argument& e) {
      logger->coreLogger->error(
          "AudioWorkspace cannot load audio file at {}, error: {}.", filePath,
          e.what());
      return;
    }

    logger->coreLogger->info("AudioWorkspace loaded audio file {}.", filePath);

    // Bind audio object to the player.
    m_player->loadAudioObject(m_audioObject);

    // Call the callback list.
    s_onAudioLoaded(m_audioObject);
  });
  logger->coreLogger->debug("Audio loading thread started.");
  loadThread.detach();
}

}  // namespace hpaslt
