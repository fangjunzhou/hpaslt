#include "commands.h"

#include "logger/logger.h"

namespace hpaslt {

std::shared_ptr<Commands> Commands::s_commands = nullptr;

Commands::Commands() {
  m_system = std::make_shared<csys::System>();
  logger->coreLogger->debug("Commands created csys::System.");
}

Commands::~Commands() {
  m_system = nullptr;
  logger->coreLogger->debug("Commands destroyed csys::System.");
}

} // namespace hpaslt
