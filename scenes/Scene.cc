#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Scene.h"
#include "../fonts/FontAtlasManager.h"
#include "../shaders/ShaderManager.h"

namespace term_engine::scenes {
  Scene::Scene() {
    glyph_set_ = std::make_shared<glyphs::GlyphSet>(fonts::GetFontAtlas(std::string(fonts::DEFAULT_FONT)), shaders::GetShader("glyph"));
  }

  void Scene::Render() const {
    glyph_set_->Render();
  }

  glyphs::GlyphSetPtr Scene::GetGlyphSet() {
    return glyph_set_;
  }
}
