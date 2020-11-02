/*
 * Copyright 2020
 * James Holtom
 * MIT License
 */

#ifndef FONT_CACHE_MANAGER_H
#define FONT_CACHE_MANAGER_H

#include "FontAtlas.h"

namespace term_engine::fonts {
  FontAtlasPtr GetFontAtlas(const std::string& font_path);
  FontAtlasPtr AddFontAtlas(const std::string& font_path, const int& glyph_height);
  void RemoveFontAtlas(const std::string& font_path);

  void CleanUpFontAtlas();

  extern FT_Library font_library;
  extern FontAtlasList font_cache_list;
}

#endif // ! FONT_CACHE_MANAGER_H
