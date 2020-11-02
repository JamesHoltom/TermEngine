#include <memory>

#include "FontAtlasManager.h"

namespace term_engine::fonts {
  FontAtlasPtr GetFontAtlas(const std::string& font_path) {
    return font_cache_list.at(font_path);
  }

  FontAtlasPtr AddFontAtlas(const std::string& font_path, const int& glyph_height) {
    FontAtlasPtr new_atlas = std::make_shared<FontAtlas>(font_library, font_path, glyph_height);
    auto font_atlas = font_cache_list.emplace(std::make_pair(font_path, std::move(new_atlas)));

    return font_atlas.first->second;
  }

  void RemoveFontAtlas(const std::string& font_path) {
    font_cache_list.erase(font_path);
  }

  void CleanUpFontAtlas() {
    font_cache_list.clear();
  }

  FT_Library font_library;
  FontAtlasList font_cache_list;
}
