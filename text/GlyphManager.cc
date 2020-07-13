#include <string>
#include <spdlog/spdlog.h>

#include "GlyphManager.h"
#include "../shaders/ShaderManager.h"

namespace term_engine::glyphs {
  void InitExampleGlyphSet() {
    GlyphSetPtr glyph_set = AddGlyphSet("example", 5, 5);

    int x = 0, y = 0;
    glm::vec2 scale = glm::vec2(0.2f, 0.2f);

    /*
     * Colors are as follows:
     * White      -> Red
     * Green      -> Blue
     * Yellow     -> Aqua
     * Magenta    -> White
     * Light Grey -> Dark Grey
     */
    const GlyphData glyph_data[25] = {
      // White -> Red
      { glm::vec2(-0.8f, -0.8f), scale, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f) },
      { glm::vec2(-0.4f, -0.8f), scale, glm::vec4(1.0f, 0.75f, 0.75f, 1.0f) },
      { glm::vec2(0.0f, -0.8f), scale, glm::vec4(1.0f, 0.5f, 0.5f, 1.0f) },
      { glm::vec2(0.4f, -0.8f), scale, glm::vec4(1.0f, 0.25f, 0.25f, 1.0f) },
      { glm::vec2(0.8f, -0.8f), scale, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) },
      // Green -> Blue
      { glm::vec2(-0.8f, -0.4f), scale, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f) },
      { glm::vec2(-0.4f, -0.4f), scale, glm::vec4(0.0f, 0.75f, 0.25f, 1.0f) },
      { glm::vec2(0.0f, -0.4f), scale, glm::vec4(0.0f, 0.5f, 0.5f, 1.0f) },
      { glm::vec2(0.4f, -0.4f), scale, glm::vec4(0.0f, 0.25f, 0.75f, 1.0f) },
      { glm::vec2(0.8f, -0.4f), scale, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f) },
      // Yellow -> Aqua
      { glm::vec2(-0.8f, 0.0f), scale, glm::vec4(1.0f, 1.0f, 0.0f, 1.0f) },
      { glm::vec2(-0.4f, 0.0f), scale, glm::vec4(0.75f, 1.0f, 0.25f, 1.0f) },
      { glm::vec2(0.0f, 0.0f), scale, glm::vec4(0.5f, 1.0f, 0.5f, 1.0f) },
      { glm::vec2(0.4f, 0.0f), scale, glm::vec4(0.25f, 1.0f, 0.75f, 1.0f) },
      { glm::vec2(0.8f, 0.0f), scale, glm::vec4(0.0f, 1.0f, 1.0f, 1.0f) },
      // Magenta -> White
      { glm::vec2(-0.8f, 0.4f), scale, glm::vec4(1.0f, 0.0f, 1.0f, 1.0f) },
      { glm::vec2(-0.4f, 0.4f), scale, glm::vec4(1.0f, 0.25f, 1.0f, 1.0f) },
      { glm::vec2(0.0f, 0.4f), scale, glm::vec4(1.0f, 0.5f, 1.0f, 1.0f) },
      { glm::vec2(0.4f, 0.4f), scale, glm::vec4(1.0f, 0.75f, 1.0f, 1.0f) },
      { glm::vec2(0.8f, 0.4f), scale, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f) },
      // Light Grey -> Dark Grey
      { glm::vec2(-0.8f, 0.8f), scale, glm::vec4(0.8f, 0.8f, 0.8f, 1.0f) },
      { glm::vec2(-0.4f, 0.8f), scale, glm::vec4(0.65f, 0.65f, 0.65f, 1.0f) },
      { glm::vec2(0.0f, 0.8f), scale, glm::vec4(0.5f, 0.5f, 0.5f, 1.0f) },
      { glm::vec2(0.4f, 0.8f), scale, glm::vec4(0.35f, 0.35f, 0.35f, 1.0f) },
      { glm::vec2(0.8f, 0.8f), scale, glm::vec4(0.2f, 0.2f, 0.2f, 1.0f) },
    };

    glyph_set->StreamData(0, 25, glyph_data);

    spdlog::info("Created example glyph set with {} instances.", glyph_set->GetCount());
  }

  GlyphSetPtr GetGlyphSet(const std::string& name) {
    return glyph_set_list.at(name);
  }

  GlyphSetPtr AddGlyphSet(const std::string& name, const int& width, const int& height) {
    auto glyph_set = glyph_set_list.emplace(std::make_pair(name, std::make_shared<GlyphSet>(width, height)));

    return glyph_set.first->second;
  }

  void RemoveGlyphSet(const std::string& name) {
    glyph_set_list.erase(name);
  }

  void CleanUpGlyphSets() {
    glyph_set_list.clear();
    GlyphSet::CleanUpVertices();
  }

  GlyphSetMap glyph_set_list;
}
