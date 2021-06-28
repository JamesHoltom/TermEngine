#include "Glyph.h"
#include "../fonts/FontAtlas.h"

namespace term_engine {
  GlyphParams default_glyph;

  void BufferData::Set(const GlyphParams& params, const bool& normalised)
  {
    glBindTexture(GL_TEXTURE_2D_ARRAY, fonts::texture_id);
    texture_layer_ = (GLfloat)fonts::GetCharacter(params.character_);

    if (!normalised) {
      foreground_color_ = params.foreground_color_ / glm::vec3(255.0f);
      background_color_ = params.background_color_ / glm::vec3(255.0f);
    }
    else {
      foreground_color_ = params.foreground_color_;
      background_color_ = params.background_color_;
    }
  }

  void BufferData::SetPosition(const glm::vec2& position)
  {
    position_ = position;
  }
}
