#include <memory>

#include "FontAtlasManager.h"
#include "../logging/Logger.h"
#include "../utility/FTUtils.h"

namespace term_engine::fonts {
  FontAtlasList font_atlas_list;

  bool FontAtlasExists(const FontAtlasKey& key)
  {
    return font_atlas_list.find(key) != font_atlas_list.end();
  }

  FontAtlasPtr GetFontAtlas(const FontAtlasKey& key)
  {
    auto result = font_atlas_list.find(key);

    if (result == font_atlas_list.end()) {
      logging::logger->warn("Could not find font atlas \"{}\" with size {}.", key.first, key.second);

      return nullptr;
    }
    else {
      return result->second;
    }
  }

  FontAtlasPtr AddFontAtlas(const FontAtlasKey& key)
  {
    auto font_atlas = font_atlas_list.emplace(std::make_pair(key, std::make_shared<FontAtlas>(FT::font_library, key.first, key.second)));

    if (font_atlas.second) {
      logging::logger->info("Added font atlas \"{}\" with size {}.", key.first, key.second);
    }

    return font_atlas.first->second;
  }

  void RemoveFontAtlas(const FontAtlasKey& key) {
    font_atlas_list.erase(key);
  }

  void CleanUpFontAtlas() {
    font_atlas_list.clear();
  }

  void GetPointerUsage() {
    for (FontAtlasIter font : font_atlas_list) {
      logging::logger->info("Font \"{}\" with size {} has {} refs.", font.first.first, font.first.second, font.second.use_count());
    }
  }
}
