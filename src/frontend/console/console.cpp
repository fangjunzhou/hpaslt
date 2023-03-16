#include "console.h"

#include <iostream>
#include <regex>
#include <sstream>
#include <string>

#include "commands/commands.h"
#include "logger/logger.h"

namespace hpaslt {

eventpp::CallbackList<void(bool)> Console::s_onEnable;

ConsoleWindow::ConsoleWindow(std::weak_ptr<csys::System> system)
    : ImGuiConsole("Console", 256, system.lock().get()) {}

Console::Console() : ImGuiObject("Console") {
  // Initialize the console system with commands system.
  m_consoleWindow = std::make_unique<ConsoleWindow>(
      Commands::getSingleton().lock()->getSystem());

  setupEnableCallback(s_onEnable);
}

Console::~Console() { resetEnableCallback(s_onEnable); }

void Console::render() {
  // TODO: Use separate thread to parse log level.
  // Convert the console ostringstream to istringstream.
  std::unique_ptr<std::istringstream> iss = nullptr;
  {
    auto oss = logger->getConsoleOutputStream().lock();
    if (oss) {
      std::string rawLine = oss->str();
      oss->str("");
      iss = std::make_unique<std::istringstream>(rawLine);
    }
  }
  // Disaply all log.
  if (iss) {
    std::string line;
    while (std::getline(*iss, line)) {
      // Default log type.
      csys::ItemType logLevel = csys::ItemType::LOG;

      std::regex levelRegex(R"(\[((debug)|(info)|(warning)|(error))\])");
      std::smatch match;
      if (std::regex_search(line, match, levelRegex)) {
        if (match[1].str() == "debug") {
          logLevel = csys::ItemType::LOG;
        } else if (match[1].str() == "info") {
          logLevel = csys::ItemType::INFO;
        } else if (match[1].str() == "warning") {
          logLevel = csys::ItemType::WARNING;
        } else if (match[1].str() == "error") {
          logLevel = csys::ItemType::ERROR;
        }
      }

      m_consoleWindow->System().Log(logLevel) << line << csys::endl;
    }
  }

  m_consoleWindow->Draw();
}

}  // namespace hpaslt
