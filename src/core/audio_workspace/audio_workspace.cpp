#include "audio_workspace.h"

#include "commands/commands.h"
#include "logger/logger.h"

namespace hpaslt {

std::unordered_map<std::string, std::shared_ptr<AudioWorkspace>>
    AudioWorkspace::s_audioWorkspacecs;

std::shared_ptr<AudioWorkspace> AudioWorkspace::s_currAudioWorkspace = nullptr;

std::weak_ptr<AudioWorkspace> AudioWorkspace::getCurrAudioWorkspace() {
  return s_currAudioWorkspace;
}

void AudioWorkspace::createAudioWorkspace(const std::string& workspaceName) {
  // Check if the audio workspace with same name exists.
  if (s_audioWorkspacecs.find(workspaceName) != s_audioWorkspacecs.end()) {
    logger->coreLogger->error("Duplicate workspace name.");
    return;
  }

  std::shared_ptr newWorkspace =
      std::make_shared<AudioWorkspace>(workspaceName);
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

void AudioWorkspace::registerConosleCommands() {
  // Get the console system.
  std::shared_ptr<csys::System> system =
      Commands::getSingleton().lock()->getSystem().lock();

  system->RegisterCommand(
      "loadAudioCurr", "Load audio to current workspace.",
      [](const csys::String& audioFilePath) {
        std::shared_ptr<AudioWorkspace> currWorkspace =
            AudioWorkspace::getCurrAudioWorkspace().lock();

        currWorkspace->loadAudioFile(audioFilePath.m_String);
      },
      csys::Arg<csys::String>("path"));

  logger->coreLogger->debug("AudioWorkspace commands registered.");
}

AudioWorkspace::AudioWorkspace(const std::string& workspaceName)
    : m_workspaceName(workspaceName) {}

AudioWorkspace::~AudioWorkspace() {}

void AudioWorkspace::loadAudioFile(const std::string& filePath) {
  m_audioFile.load(filePath);
  logger->coreLogger->info("AudioWorkspace {} loaded audio file {}.",
                           m_workspaceName, filePath);
}

}  // namespace hpaslt
