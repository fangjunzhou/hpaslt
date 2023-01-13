#include <thread>

#include "audio_workspace.h"

#include "commands/commands.h"
#include "logger/logger.h"

namespace hpaslt {

std::unordered_map<std::string, AudioWorkspace*>
    AudioWorkspace::s_audioWorkspacecs;

AudioWorkspace* AudioWorkspace::s_currAudioWorkspace = nullptr;

AudioWorkspace* AudioWorkspace::getCurrAudioWorkspace() {
  return s_currAudioWorkspace;
}

void AudioWorkspace::createAudioWorkspace(const std::string& workspaceName) {
  // Check if the audio workspace with same name exists.
  if (s_audioWorkspacecs.find(workspaceName) != s_audioWorkspacecs.end()) {
    logger->coreLogger->error("Duplicate workspace name.");
    return;
  }

  AudioWorkspace* newWorkspace = new AudioWorkspace(workspaceName);
  s_audioWorkspacecs[workspaceName] = newWorkspace;
  logger->coreLogger->info("AudioWorkspace {} created.", workspaceName);
}

void AudioWorkspace::changeAudioWorkspace(const std::string& workspaceName) {
  // Check if the audio workspace with same name exists.
  if (s_audioWorkspacecs.find(workspaceName) == s_audioWorkspacecs.end()) {
    logger->coreLogger->error("Worksapce with name {} not found.",
                              workspaceName);
    return;
  }

  s_currAudioWorkspace = s_audioWorkspacecs[workspaceName];
  logger->coreLogger->info("Switched to AudioWorkspace {}.", workspaceName);
}

void AudioWorkspace::cleanupAudioWorkspace() {
  std::for_each(s_audioWorkspacecs.begin(), s_audioWorkspacecs.end(),
                [](std::pair<std::string, AudioWorkspace*> key_value) {
                  delete key_value.second;
                });
  s_audioWorkspacecs.clear();
  logger->coreLogger->debug("AudioWorkspaces cleaned up.");
}

void AudioWorkspace::registerConosleCommands() {
  // Get the console system.
  std::shared_ptr<csys::System> system =
      Commands::getSingleton().lock()->getSystem().lock();

  system->RegisterCommand(
      "loadAudioCurr", "Load audio to current workspace.",
      [](const csys::String& audioFilePath) {
        AudioWorkspace* currWorkspace = AudioWorkspace::getCurrAudioWorkspace();

        currWorkspace->loadAudioFile(audioFilePath.m_String);
      },
      csys::Arg<csys::String>("path"));

  system->RegisterCommand(
      "playAudioCurr", "Play the audio of current workspace.", []() {
        AudioWorkspace* currWorkspace = AudioWorkspace::getCurrAudioWorkspace();

        currWorkspace->m_player->play();
      });

  system->RegisterCommand(
      "pauseAudioCurr", "Pause the audio of current workspace.", []() {
        AudioWorkspace* currWorkspace = AudioWorkspace::getCurrAudioWorkspace();

        currWorkspace->m_player->pause();
      });

  logger->coreLogger->debug("AudioWorkspace commands registered.");
}

AudioWorkspace::AudioWorkspace(const std::string& workspaceName)
    : m_workspaceName(workspaceName) {
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
          "AudioWorkspace {} cannot load audio file at {}.", m_workspaceName,
          filePath);
      return;
    }

    logger->coreLogger->info("AudioWorkspace {} loaded audio file {}.",
                             m_workspaceName, filePath);

    // Bind audio object to the player.
    m_player->loadAudioObject(m_audioObject);
  });
  logger->coreLogger->debug("Audio loading thread started.");
  loadThread.detach();
}

}  // namespace hpaslt
