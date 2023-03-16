#include "play_control.h"

#include <IconsMaterialDesign.h>
#include <imgui.h>

#include <iomanip>
#include <sstream>

#include "core/audio_workspace/audio_workspace.h"

namespace hpaslt {

void PlayControl::playPauseSwitch() {
  if (!m_isPlaying) {
    logger->coreLogger->trace("Play");
    AudioWorkspace::getSingleton().lock()->getAudioPlayer().lock()->play();
  } else {
    logger->coreLogger->trace("Pause");
    AudioWorkspace::getSingleton().lock()->getAudioPlayer().lock()->pause();
  }
}

PlayControl::PlayControl()
    : ImGuiObject("MainPlayConstrol"),
      m_isPlaying(false),
      m_currTime(0),
      m_sliderTime(0),
      m_syncSliderTime(true),
      m_totalTime(0) {
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
    if (ImGui::MenuItem(playIcon.c_str())) playPauseSwitch();
    // Space key play control.
    if (ImGui::IsKeyPressed(ImGuiKey_Space)) playPauseSwitch();

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
    const ImGuiSliderFlags playTimeSliderFlag =
        ImGuiSliderFlags_NoInput | ImGuiSliderFlags_NoRoundToFormat;
    std::stringstream playingTimeStrStream;
    playingTimeStrStream << std::fixed << std::setprecision(2) << m_totalTime;
    // Sync play time.
    if (m_syncSliderTime) {
      m_sliderTime = m_currTime;
    }
    // Slider.
    ImGui::PushItemWidth(-100);
    ImGui::SliderFloat("", &m_sliderTime, 0, m_totalTime, "%.2f",
                       playTimeSliderFlag);
    if (ImGui::IsItemActivated()) {
      // Disable play slider synchronization.
      m_syncSliderTime = false;
    }
    if (ImGui::IsItemDeactivated()) {
      // Update play time.
      m_currTime = m_sliderTime;
      AudioWorkspace::getSingleton().lock()->getAudioPlayer().lock()->setTime(
          m_sliderTime);
      AudioWorkspace::getSingleton()
          .lock()
          ->getAudioPlayer()
          .lock()
          ->getOnChangePlayingTime()(m_sliderTime, m_totalTime);

      // Enable play slider synchronization.
      m_syncSliderTime = true;
    }

    // Total time.
    auto windowWidth = ImGui::GetWindowSize().x;
    auto textWidth = ImGui::CalcTextSize(playingTimeStrStream.str().c_str()).x;
    ImGui::SetCursorPosX(windowWidth - 50 - textWidth * 0.5f);
    ImGui::PushItemWidth(100);
    ImGui::Text(playingTimeStrStream.str().c_str());

    ImGui::EndMenuBar();
  }

  ImGui::End();
}

}  // namespace hpaslt
