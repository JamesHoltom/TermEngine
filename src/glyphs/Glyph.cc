#include "Glyph.h"
#include "../data/Uniform.h"
#include "../fonts/FontAtlas.h"
#include "../utility/DebugFunctions.h"

namespace term_engine::glyphs {
  GlyphParams::GlyphParams(const char& character, const glm::vec4& foreground_color, const glm::vec4& background_color) :
    character_(character),
    foreground_color_(foreground_color),
    background_color_(background_color)
  {}

  BufferData::BufferData() :
    texture_layer_(0.0f),
    position_(glm::vec2(0.0f)),
    foreground_color_(DEFAULT_FOREGROUND_COLOR),
    background_color_(DEFAULT_BACKGROUND_COLOR)
  {}

  BufferData::BufferData(const GLfloat& texture_layer, const glm::vec2& position, const glm::vec4& foreground_color, const glm::vec4& background_color) :
    texture_layer_(texture_layer),
    position_(position),
    foreground_color_(foreground_color),
    background_color_(background_color)
  {}

  void BufferData::Set(const GlyphParams& params, const bool& normalised)
  {
    texture_layer_ = (GLfloat)fonts::GetCharacter(params.character_);

    if (!normalised) {
      foreground_color_ = params.foreground_color_ / glm::vec4(255.0f);
      background_color_ = params.background_color_ / glm::vec4(255.0f);
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

  int Init()
  {
    dimensions = glm::uvec2(32, 16);
    data.reserve((size_t)dimensions.x * (size_t)dimensions.y);
    data.resize((size_t)dimensions.x * (size_t)dimensions.y);

    glm::vec2 font_size = data::GetFontSize();
    int count = 0;

    for (glyphs::BufferData& glyph : data) {
      glyph.foreground_color_ = DEFAULT_FOREGROUND_COLOR;
      glyph.position_ = glm::vec2((count % dimensions.x) * font_size.x, (count / dimensions.x) * font_size.y);

      ++count;
    }

    glGenVertexArrays(1, &vao_id);
    glBindVertexArray(vao_id);

    glGenBuffers(1, &vbo_id);
    glBindVertexBuffer(0, vbo_id, 0, sizeof(BufferData));

    // Configure the texture layer attribute.
    glEnableVertexAttribArray(0);
    glVertexAttribFormat(0, 1, GL_FLOAT, GL_FALSE, offsetof(BufferData, texture_layer_));
    glVertexAttribBinding(0, 0);

    // Configure the glyph position attribute.
    glEnableVertexAttribArray(1);
    glVertexAttribFormat(1, 2, GL_FLOAT, GL_FALSE, offsetof(BufferData, position_));
    glVertexAttribBinding(1, 0);

    // Configure the foreground colour attribute.
    glEnableVertexAttribArray(2);
    glVertexAttribFormat(2, 4, GL_FLOAT, GL_FALSE, offsetof(BufferData, foreground_color_));
    glVertexAttribBinding(2, 0);

    // Configure the background colour attribute.
    glEnableVertexAttribArray(3);
    glVertexAttribFormat(3, 4, GL_FLOAT, GL_FALSE, offsetof(BufferData, background_color_));
    glVertexAttribBinding(3, 0);

    debug::LogVAOData();

    glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(BufferData) * data.capacity(), data.data(), GL_DYNAMIC_DRAW);

    glBindVertexArray(0);

    return 0;
  }

  void CleanUp()
  {
    data.clear();
    glDeleteVertexArrays(1, &vao_id);
    glDeleteBuffers(1, &vbo_id);
  }

  void Render()
  {
    glBindVertexArray(vao_id);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(BufferData) * data.capacity(), data.data(), GL_DYNAMIC_DRAW);

    glDrawArrays(GL_POINTS, 0, static_cast<GLsizei>(dimensions.x * dimensions.y));
  }

  GLuint vao_id;
  GLuint vbo_id;
  glm::uvec2 dimensions;
  BufferList data;
  GlyphParams default_glyph = GlyphParams();
}
