#include <iomanip>
#include <sstream>

#include <imgui.h>

#include <IconsMaterialDesign.h>

#include "play_control.h"

#include "core/audio_workspace/audio_workspace.h"

namespace hpaslt {

PlayControl::PlayControl()
    : ImGuiObject("MainPlayConstrol"), m_isPlaying(false) {
  m_onPlayingStatusChangedHandle =
      AudioWorkspace::getSingleton()
          .lock()
          ->getAudioPlayer()
          .lock()
          ->getOnChangePlayingStatus()
          .append([this](bool isPlaying) { m_isPlaying = isPlaying; });

  m_onPlayingTimeChangedHandle =
      AudioWorkspace::getSingleton()
          .lock()
          ->getAudioPlayer()
          .lock()
          ->getOnChangePlayingTime()
          .append([this](float currTime, float totalTime) {
            m_currTime = currTime;
            m_totalTime = totalTime;
          });
}

PlayControl::~PlayControl() {
  AudioWorkspace::getSingleton()
      .lock()
      ->getAudioPlayer()
      .lock()
      ->getOnChangePlayingStatus()
      .remove(m_onPlayingStatusChangedHandle);

  AudioWorkspace::getSingleton()
      .lock()
      ->getAudioPlayer()
      .lock()
      ->getOnChangePlayingTime()
      .remove(m_onPlayingTimeChangedHandle);
}

void PlayControl::render() {
  // Status bar window.
  const ImGuiViewport* viewport = ImGui::GetMainViewport();
  ImVec2 statusPos = viewport->Pos;
  statusPos.y += ImGui::GetFrameHeight();
  ImGui::SetNextWindowPos(statusPos);
  // Resize the work space for status bar.
  ImVec2 workSize = viewport->Size;
  workSize.y = ImGui::GetFrameHeight();
  ImGui::SetNextWindowSize(workSize);
  ImGui::SetNextWindowViewport(viewport->ID);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, workSize);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
  // Disable padding.
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

  // Add menu bar flag and disable everything else
  // Disable docking to other dockspace.
  ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
  // Fix window.
  window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
                  ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
  // Disable nav focus.
  window_flags |=
      ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
  // Enable menu bar.
  window_flags |= ImGuiWindowFlags_MenuBar;

  ImGui::Begin("PlayControl", nullptr, window_flags);
  ImGui::PopStyleVar(4);

  if (ImGui::BeginMenuBar()) {
    ImGui::Text("Play Control");

    // Play.
    std::string playIcon = m_isPlaying ? ICON_MD_PAUSE : ICON_MD_PLAY_ARROW;
    if (ImGui::MenuItem(playIcon.c_str())) {
      if (!m_isPlaying) {
        logger->coreLogger->trace("Play");
        AudioWorkspace::getSingleton().lock()->getAudioPlayer().lock()->play();
      } else {
        logger->coreLogger->trace("Pause");
        AudioWorkspace::getSingleton().lock()->getAudioPlayer().lock()->pause();
      }
    }

    // Replay.
    if (ImGui::MenuItem(ICON_MD_REPLAY)) {
      logger->coreLogger->trace("Replay");
      AudioWorkspace::getSingleton().lock()->getAudioPlayer().lock()->replay();
    }

    // Stop.
    if (ImGui::MenuItem(ICON_MD_STOP)) {
      logger->coreLogger->trace("Stop");
      AudioWorkspace::getSingleton().lock()->getAudioPlayer().lock()->stop();
    }

    // Progress slider.
    std::stringstream playingTimeStrStream;
    playingTimeStrStream << std::fixed << std::setprecision(2) << m_currTime;
    playingTimeStrStream << "/";
    playingTimeStrStream << std::fixed << std::setprecision(2) << m_totalTime;
    if (ImGui::SliderFloat(playingTimeStrStream.str().c_str(), &m_currTime, 0,
                           m_totalTime)) {
    }

    ImGui::EndMenuBar();
  }

  ImGui::End();
}

}  // namespace hpaslt
