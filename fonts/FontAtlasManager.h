/*
 * Copyright 2020
 * James Holtom
 * MIT License
 */

#ifndef FONT_CACHE_MANAGER_H
#define FONT_CACHE_MANAGER_H

#include <string>

#include "FontAtlas.h"

namespace term_engine::fonts {
  constexpr char DEFAULT_FONT[] = "resources/fonts/lucon.ttf";
  constexpr int DEFAULT_FONT_SIZE = 16;

  FontAtlasPtr GetFontAtlas(const std::string& font_path);
  FontAtlasPtr AddFontAtlas(const std::string& font_path, const int& glyph_height);
  void RemoveFontAtlas(const std::string& font_path);

  void CleanUpFontAtlas();

  extern FT_Library font_library;
  extern FontAtlasList font_atlas_list;
}

#endif // ! FONT_CACHE_MANAGER_H
