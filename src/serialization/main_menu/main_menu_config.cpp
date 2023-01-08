#include <fstream>

#include "main_menu_config.h"

namespace hpaslt {

void MainMenuConfig::save() {
  // Create the file stream.
  std::ofstream fs(m_savePath);
  // Create the cereal archive.
  cereal::JSONOutputArchive jsonArchive(fs);

  // Serialize the data.
  jsonArchive(*this);
}

void MainMenuConfig::load() {
  // Create the file stream.
  std::ifstream fs(m_savePath);
  // Create the cereal archive.
  cereal::JSONInputArchive jsonArchive(fs);

  // Deserialize the data.
  jsonArchive(*this);
}

}  // namespace hpaslt
