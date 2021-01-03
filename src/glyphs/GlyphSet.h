/*
 * Copyright 2020
 * James Holtom
 * MIT License
 */

#ifndef GLYPH_SET_H
#define GLYPH_SET_H

#include <memory>
#include <string>

#include "Glyph.h"
#include "../fonts/FontAtlas.h"
#include "../shaders/Shader.h"
#include "../utility/GLUtils.h"

namespace term_engine::glyphs {
  constexpr int MAX_GLYPHS = 16384;
  constexpr int VERTEX_COUNT = 6;
  constexpr int DEFAULT_SET_WIDTH = 32;
  constexpr int DEFAULT_SET_HEIGHT = 16;

  typedef struct {
    glm::vec2 vertex_position;
    glm::vec2 texture_position;
  } VertexData;

  class GlyphSet {
  public:
    GlyphSet() = delete;
    GlyphSet(const fonts::FontAtlasPtr& font, const shaders::ShaderPtr& shader);
    ~GlyphSet();

    glm::vec2 GetPosition() const;
    glm::vec2 GetScale() const;
    glm::vec2 GetPadding() const;
    glm::vec2 GetSpacing() const;
    glm::uvec2 GetSize() const;
    std::string GetFontPath() const;
    void SetPosition(const glm::vec2& position);
    void SetScale(const glm::vec2& scale);
    void SetPadding(const glm::vec2& padding);
    void SetSpacing(const glm::vec2& spacing);
    void SetSize(const glm::uvec2& size);
    void SetFont(const std::string& font_path);
    void SetFont(const fonts::FontAtlasPtr& font);
    void SetShader(const std::string& name);
    void SetShader(const shaders::ShaderPtr& shader);

    size_t GetCount() const;

    GlyphData& GetGlyph(const glm::uvec2& index);

    void WriteLine(const glm::ivec2& index, const std::string& text, const glm::vec4& fg_color, const glm::vec4& bg_color);

    void Clear(GlyphData& glyph);
    void ClearAll();
    void ResetOffset(GlyphData& glyph);
    void ResetAllOffsets();
    void ResetScale();
    void ResetProjection();

    void PrintData() const;

    void Dirty();
    void Render();

  private:
    bool is_dirty_;

    GLuint vao_id_;
    GLuint vbo_id_;

    glm::vec2 set_position_;
    glm::vec2 set_scale_;
    glm::vec2 set_padding_;
    glm::vec2 set_spacing_;
    glm::uvec2 set_size_;
    GlyphList set_data_;
    fonts::FontAtlasPtr set_font_;
    shaders::ShaderPtr set_shader_;

    void InitData();
    void InitBuffers();
    void SetGLBuffer() const;

    glm::vec2 GetOffset(const GlyphData& glyph) const;

    void PrintBuffer(const BufferList& data) const;
  };

  void InitVertexData();
  void CleanUpVertexData();

  extern GLuint vertex_vbo_id;
  extern GLuint vertex_ebo_id;

  typedef std::shared_ptr<GlyphSet> GlyphSetPtr;
}

#endif // ! GLYPH_SET_H
