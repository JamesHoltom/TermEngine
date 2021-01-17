#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Scene.h"
#include "../fonts/FontAtlasManager.h"
#include "../shaders/ShaderManager.h"
#include "../logging/Logger.h"

namespace term_engine::scenes {
  Scene::Scene()
  {
    glyph_set_ = std::make_shared<glyphs::GlyphSet>(fonts::GetFontAtlas(std::string(fonts::DEFAULT_FONT)), shaders::GetShader("glyph"));
    logging::logger->info("Scene: There are {} refs.", glyph_set_.use_count());
  }

  Scene::~Scene()
  {
    glyph_set_.reset();
    logging::logger->info("~Scene: There are {} refs.", glyph_set_.use_count());
  }

  void Scene::Render() const
  {
    glyph_set_->Render();
  }

  glyphs::GlyphSetPtr Scene::GetGlyphSet()
  {
    return glyph_set_;
  }
}
