/*
 * Copyright 2020
 * James Holtom
 * MIT License
 */

#ifndef GLYPH_MANAGER_H
#define GLYPH_MANAGER_H

#include <string>

#include "GlyphSet.h"
#include "../utility/spdlogUtils.h"

namespace term_engine::glyphs {
  GlyphSetPtr GetGlyphSet(const std::string& name);
  GlyphSetPtr AddGlyphSet(const std::string& name, const int& width, const int& height);
  void RemoveGlyphSet(const std::string& name);

  void CleanUpGlyphSets();

  extern GlyphSetMap glyph_set_list;
}

#endif
