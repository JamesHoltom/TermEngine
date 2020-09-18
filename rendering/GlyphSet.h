/*
 * Copyright 2020
 * James Holtom
 * MIT License
 */

#ifndef GLYPH_SET_H
#define GLYPH_SET_H

#include <iterator>
#include <memory>
#include <string>
#include <tuple>
#include <vector>

#include "../gl_includes.h"
#include "../fonts/FontAtlas.h"

namespace term_engine::glyphs {
  constexpr int MAX_GLYPHS = 16384;

  typedef struct {
    glm::vec2 instance_offset;
    glm::vec2 instance_scale;
    glm::vec4 texture_bounds;
    glm::vec4 foreground_color;
    glm::vec4 background_color;
  } GlyphData;

  typedef std::vector<char> GlyphCharList;
  typedef std::vector<GlyphData> GlyphDataList;

  typedef std::unique_ptr<GlyphCharList> GlyphCharPtr;
  typedef std::unique_ptr<GlyphDataList> GlyphDataPtr;

  typedef std::tuple<char, GlyphData> GlyphIter;

  class GlyphSet {
  public:
    GlyphSet(const std::string& font, const int& pt_size, const int& width, const int& height);
    ~GlyphSet();

    char GetChar(const int& x, const int& y) const;
    GlyphData GetData(const int& x, const int& y) const;
    glm::ivec2 GetSize() const;
    size_t GetCount() const;

    void SetChar(const int& x, const int& y, const char& glyph, fonts::FontAtlasPtr& font_atlas);
    void SetOffset(const int& x, const int& y, const glm::vec2& offset);
    void SetScale(const int& x, const int& y, const glm::vec2& scale);
    void SetFGColor(const int& x, const int& y, const glm::vec4& fg_color);
    void SetBGColor(const int& x, const int& y, const glm::vec4& bg_color);
    void SetData(const int& x, const int& y, const GlyphData& data);
    void SetSize(const int& width, const int& height);

    void Render();

  private:
    bool is_dirty_;

    GLuint vao_id_;
    GLuint instance_vbo_id_;

    size_t set_width_;
    size_t set_height_;

    GlyphCharPtr set_chars_;
    GlyphDataPtr set_data_;

    void InitData();
    void InitBuffers();
    void SetGLBuffer() const;
  };

  void InitVertices();
  void CleanUpVertices();

  extern GLuint vertice_vbo_id;
  extern GLuint vertice_ebo_id;

  extern const glm::vec2 vertices[4];
  extern const GLuint elements[6];
}

#endif
