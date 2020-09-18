#include <string>
#include <spdlog/spdlog.h>

#include "GlyphManager.h"
#include "../shaders/ShaderManager.h"

namespace term_engine::glyphs {
  GlyphSetPtr GetGlyphSet(const std::string& name) {
    return glyph_set_list.at(name);
  }

  GlyphSetPtr AddGlyphSet(const std::string& name, const int& width, const int& height) {
    auto glyph_set = glyph_set_list.emplace(std::make_pair(name, std::make_shared<GlyphSet>("C:\\Windows\\Fonts\\arial.ttf", 18, width, height)));

    return glyph_set.first->second;
  }

  void RemoveGlyphSet(const std::string& name) {
    glyph_set_list.erase(name);
  }

  void CleanUpGlyphSets() {
    glyph_set_list.clear();
    CleanUpVertices();
  }

  GlyphSetMap glyph_set_list;
}
