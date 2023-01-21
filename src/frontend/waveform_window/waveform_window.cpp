#include <imgui.h>
#include <implot.h>

#include "core/audio_workspace/audio_workspace.h"
#include "waveform_window.h"

#define AUDIO_WAVEFORM_RESOLUTION 8192

namespace hpaslt {

eventpp::CallbackList<void(bool)> WaveformWindow::s_onEnable;

WaveformWindow::WaveformWindow()
    : ImGuiObject("Waveform"),
      m_channelNum(0),
      m_sampleRate(0),
      m_sampleSize(0) {
  // Setup window enable callback.
  setupEnableCallback(s_onEnable);

  // Setuo audio loaded callback.
  m_audioLoadedHandle = AudioWorkspace::s_onAudioLoaded.append(
      [&](std::weak_ptr<AudioObject> audioObj) {
        logger->coreLogger->trace("WaveformWindow load new AudioObject.");

        // Load m_audioObj.
        m_audioMutex.lock();

        m_audioObj = audioObj.lock();

        // Copy all the audio data
        m_channelNum = m_audioObj->getAudioFile().getNumChannels();
        m_sampleRate = m_audioObj->getAudioFile().getSampleRate();
        m_sampleSize = m_audioObj->getAudioFile().getNumSamplesPerChannel();
        auto& samples = m_audioObj->getAudioFile().samples;
        logger->coreLogger->trace("WaveformWindow copied audio data.");

        m_audioChannels.clear();
        AudioChannel audioChannel;
        for (int channel = 0; channel < m_channelNum; channel++) {
          audioChannel.wx = std::make_shared<std::vector<float>>(m_sampleSize);
          audioChannel.wy = std::make_shared<std::vector<float>>(m_sampleSize);
          audioChannel.size = m_sampleSize;
          for (int i = 0; i < audioChannel.size; i++) {
            (*audioChannel.wx)[i] = (float)i / (float)m_sampleRate;
            (*audioChannel.wy)[i] = samples[channel][i];
          }
          m_audioChannels.push_back(audioChannel);
        }
        logger->coreLogger->trace("WaveformWindow generated {} channel data.",
                                  m_channelNum);

        m_audioMutex.unlock();
      });
}

WaveformWindow::~WaveformWindow() {
  // Reset window enable callback.
  resetEnableCallback(s_onEnable);
  // Remove audio loaded callback.
  AudioWorkspace::s_onAudioLoaded.remove(m_audioLoadedHandle);
}

void WaveformWindow::render() {
  // Init window properties.
  const ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar;
  ImGui::SetNextWindowSize(ImVec2(500, 440), ImGuiCond_FirstUseEver);
  ImGui::Begin("Waveform", nullptr, windowFlags);

  if (m_audioMutex.try_lock()) {
    if (m_channelNum > 0 &&
        ImPlot::BeginSubplots("Audio Channels", m_channelNum, 1, ImVec2(-1, -1),
                              ImPlotSubplotFlags_LinkAllX)) {
      static std::array<float, AUDIO_WAVEFORM_RESOLUTION> sampleX;
      static std::array<float, AUDIO_WAVEFORM_RESOLUTION> sampleY;
      for (int channel = 0; channel < m_channelNum; channel++) {
        AudioChannel& audioChannel = m_audioChannels[channel];
        std::stringstream channelName;
        channelName << "Channel " << channel;
        if (ImPlot::BeginPlot(channelName.str().c_str())) {
          ImPlot::SetupAxes("Time", "Amplitude");
          ImPlot::SetupAxisLimits(ImAxis_Y1, -1, 1, ImPlotCond_Always);
          auto widthPix = ImPlot::GetPlotSize().x;
          int start = ImPlot::GetPlotLimits().X.Min * m_sampleRate;
          int end = ImPlot::GetPlotLimits().X.Max * m_sampleRate;
          // Clamp start and end.
          start = start < 0 ? 0 : start;
          start = start > m_sampleSize ? m_sampleSize : start;
          end = end < 0 ? 0 : end;
          end = end > m_sampleSize ? m_sampleSize : end;
          // Resample data.
          int span = end - start;
          int size = span > AUDIO_WAVEFORM_RESOLUTION
                         ? AUDIO_WAVEFORM_RESOLUTION
                         : span;
          for (int i = 0; i < size; i++) {
            int sampleIndx = start + ((float)i / (float)size) * span;
            sampleX[i] = (float)sampleIndx / (float)m_sampleRate;
            sampleY[i] = (*audioChannel.wy)[sampleIndx];
          }

          ImPlot::PlotLine("Audio", sampleX.data(), sampleY.data(), size);
          ImPlot::EndPlot();
        }
      }

      ImPlot::EndSubplots();
    }
    m_audioMutex.unlock();
  }

  ImGui::End();
}

}  // namespace hpaslt
