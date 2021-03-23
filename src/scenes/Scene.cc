#include "Scene.h"
#include "../data/Uniform.h"
#include "../fonts/FontAtlasManager.h"
#include "../logging/Logger.h"
#include "../shaders/ShaderManager.h"
#include "../utility/GLUtils.h"

namespace term_engine::scenes {
  Scene::Scene()
  {
    glyph_set_ = std::make_shared<glyphs::GlyphSet>();
    font_atlas_ = fonts::GetFontAtlas(fonts::FontAtlasKey(fonts::DEFAULT_FONT, fonts::DEFAULT_FONT_SIZE));
    shader_ = shaders::GetShader("glyph");

    const GLint active_texture = 0;
    shader_->Use();
    shader_->SetUniformInt("font_texture", 1, &active_texture);
    shader_->Unuse();

    data::SetFontSize(font_atlas_);

    logging::logger->debug("Scene: There are {} glyph refs.", glyph_set_.use_count());
    logging::logger->debug("Scene: There are {} font refs.", font_atlas_.use_count());
    logging::logger->debug("Scene: There are {} shader refs.", shader_.use_count());
  }

  Scene::~Scene()
  {
    glyph_set_.reset();
    font_atlas_.reset();
    shader_.reset();

    logging::logger->debug("~Scene: There are {} glyph refs.", glyph_set_.use_count());
    logging::logger->debug("~Scene: There are {} font refs.", font_atlas_.use_count());
    logging::logger->debug("~Scene: There are {} shader refs.", shader_.use_count());
  }

  void Scene::Render() const
  {
    glyph_set_->Render(font_atlas_, shader_);
  }

  glyphs::GlyphSetPtr Scene::GetGlyphSet() const
  {
    return glyph_set_;
  }

  fonts::FontAtlasPtr Scene::GetFont() const
  {
    return font_atlas_;
  }

  shaders::ShaderPtr Scene::GetShader() const
  {
    return shader_;
  }

  void Scene::SetFont(const fonts::FontAtlasKey& key)
  {
    if (fonts::FontAtlasExists(key)) {
      font_atlas_ = fonts::GetFontAtlas(key);
    }
    else {
      logging::logger->warn("Font \"{}\" with size of {} has not been created! Add it first before setting it in a scene.", key.first, key.second);
    }
  }

  void Scene::SetFont(const fonts::FontAtlasPtr& font)
  {
    font_atlas_ = font;
  }

  void Scene::SetShader(const std::string& shader_name)
  {
    if (shaders::ShaderExists(shader_name)) {
      shader_ = shaders::GetShader(shader_name);
    }
    else {
      logging::logger->warn("Shader \"{}\" has not been created! Add it first before setting it in a scene.", shader_name);
    }
  }

  void Scene::SetShader(const shaders::ShaderPtr& shader)
  {
    shader_ = shader;
  }
}
