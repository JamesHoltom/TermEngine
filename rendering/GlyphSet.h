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
#include <unordered_map>
#include <vector>

#include "../fonts/FontAtlas.h"
#include "../utility/GLUtils.h"

namespace term_engine::glyphs {
  constexpr int MAX_GLYPHS = 16384;
  constexpr int VERTEX_COUNT = 6;

  typedef struct {
    glm::vec2 vertex_position;
    glm::vec2 texture_position;
  } VertexData;

  typedef struct _GlyphData {
    glm::vec2 vertex_offset;
    float texture_offset;
    glm::vec4 foreground_color;
    glm::vec4 background_color;
    _GlyphData() : vertex_offset(glm::vec2(0.0f)), texture_offset(0), foreground_color(glm::vec4(1.0f)), background_color(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)) {};
  } GlyphData;

  typedef std::string GlyphCharList;
  typedef std::vector<GlyphData> GlyphDataList;

  typedef std::unique_ptr<GlyphCharList> GlyphCharPtr;
  typedef std::unique_ptr<GlyphDataList> GlyphDataPtr;

  typedef std::tuple<char, GlyphData> GlyphIter;

  class GlyphSet {
  public:
    GlyphSet(const std::string& font, const int& pt_size);
    ~GlyphSet();

    glm::ivec2 GetScale() const;
    glm::ivec2 GetPadding() const;
    glm::ivec2 GetSpacing() const;
    glm::ivec2 GetSize() const;
    void SetScale(const int& width, const int& height);
    void SetPadding(const int& x_padding, const int& y_padding);
    void SetSpacing(const int& x_spacing, const int& y_spacing);
    void SetSize(const int& width, const int& height);

    char GetChar(const int& x, const int& y) const;
    void SetChar(const int& x, const int& y, const char& glyph, fonts::FontAtlasPtr& font_atlas);
    void SetOffset(const int& x, const int& y, const glm::vec2& offset);
    void SetFGColor(const int& x, const int& y, const glm::vec4& fg_color);
    void SetBGColor(const int& x, const int& y, const glm::vec4& bg_color);

    size_t GetCount() const;

    void ResetOffset(const int& x, const int& y);
    void ResetAllOffsets();

    void Render();

  private:
    bool is_dirty_;

    GLuint vao_id_;
    GLuint vbo_id_;

    size_t set_width_;
    size_t set_height_;
    glm::ivec2 set_offset_;

    glm::ivec2 glyph_scale_;
    glm::ivec2 glyph_padding_;
    glm::ivec2 glyph_spacing_;

    std::string set_chars_;
    GlyphDataPtr set_data_;

    void InitData();
    void InitBuffers();
    void SetGLBuffer() const;

    void PrintData() const;
  };

  void InitVertexData();
  void CleanUpVertexData();

  extern GLuint vertex_vbo_id;
  extern GLuint vertex_ebo_id;

  typedef std::shared_ptr<GlyphSet> GlyphSetPtr;
  typedef std::pair<std::string, GlyphSet> GlyphSetIter;
  typedef std::unordered_map<std::string, GlyphSetPtr> GlyphSetMap;
}

#endif
