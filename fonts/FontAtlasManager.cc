#include <memory>

#include "FontAtlasManager.h"
#include "../logging/Logger.h"

namespace term_engine::fonts {
  FontAtlasPtr GetFontAtlas(const std::string& font_path) {
    auto result = font_atlas_list.find(font_path);

    if (result == font_atlas_list.end()) {
      logging::logger->warn("Could not find font atlas \"{}\".", font_path);

      return nullptr;
    }
    else {
      return result->second;
    }
  }

  FontAtlasPtr AddFontAtlas(const std::string& font_path, const int& glyph_height) {
    FontAtlasPtr new_atlas = std::make_shared<FontAtlas>(font_library, font_path, glyph_height);
    auto font_atlas = font_atlas_list.emplace(std::make_pair(font_path, std::move(new_atlas)));

    if (font_atlas.second) {
      logging::logger->info("Added font atlas \"{}\".", font_path);
    }

    return font_atlas.first->second;
  }

  void RemoveFontAtlas(const std::string& font_path) {
    font_atlas_list.erase(font_path);
  }

  void CleanUpFontAtlas() {
    font_atlas_list.clear();
  }

  FT_Library font_library;
  FontAtlasList font_atlas_list;
}
