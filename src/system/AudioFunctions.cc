#include <filesystem>
#include "AudioFunctions.h"
#include "FileFunctions.h"
#include "../utility/AudioUtils.h"

namespace term_engine::system {
  void PlaySound(const std::string& filepath)
  {
    const std::filesystem::path fullPath = SearchForResourcePath(filepath);

    if (fullPath != "")
    {
      ma_engine_play_sound(&utility::engine, fullPath.c_str(), nullptr);
    }
  }
}