#include "GlyphManager.h"
#include "../shaders/ShaderManager.h"
#include "../utility/spdlogUtils.h"

namespace term_engine::glyphs {
  GlyphSetPtr GetGlyphSet(const std::string& name) {
    return glyph_set_list.at(name);
  }

  GlyphSetPtr AddGlyphSet(const std::string& name, const int& width, const int& height) {
    auto glyph_set = glyph_set_list.emplace(std::make_pair(name, std::make_shared<GlyphSet>()));

    glyph_set.first->second->SetSize(width, height);

    return glyph_set.first->second;
  }

  void RemoveGlyphSet(const std::string& name) {
    glyph_set_list.erase(name);
  }

  void CleanUpGlyphSets() {
    glyph_set_list.clear();
  }

  GlyphSetMap glyph_set_list;
}
