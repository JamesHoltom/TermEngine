/*
 * Copyright 2020
 * James Holtom
 * MIT License
 */

#ifndef GLYPH_MANAGER_H
#define GLYPH_MANAGER_H

#include "../gl_includes.h"
#include "GlyphSet.h"

namespace term_engine::glyphs {
  typedef std::shared_ptr<GlyphSet> GlyphSetPtr;
  typedef std::pair<std::string, GlyphSet> GlyphSetIter;
  typedef std::unordered_map<std::string, GlyphSetPtr> GlyphSetMap;

  void InitExampleGlyphSet();

  GlyphSetPtr GetGlyphSet(const std::string& name);
  GlyphSetPtr AddGlyphSet(const std::string& name, const int& width, const int& height);
  void RemoveGlyphSet(const std::string& name);

  void CleanUpGlyphSets();

  extern GlyphSetMap glyph_set_list;
}

#endif
