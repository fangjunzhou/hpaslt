#include <fstream>
#include <filesystem>

#include "main_menu_config.h"
#include "logger/logger.h"

namespace hpaslt {

void MainMenuConfig::save() { saveHelper(*this); }

void MainMenuConfig::load() { loadHelper(*this); }

}  // namespace hpaslt
