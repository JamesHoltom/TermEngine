/*
 * Copyright 2020
 * James Holtom
 * MIT License
 */

#ifndef GLYPH_SET_H
#define GLYPH_SET_H

#include <string>
#include <vector>

#include "../gl_includes.h"

namespace term_engine::glyphs {
  constexpr int MAX_GLYPHS = 16384;

  typedef struct {
    glm::vec2 offset;
    glm::vec2 scale;
    glm::vec4 color;
  } GlyphData;

  class GlyphSet {
  public:
    GlyphSet();
    GlyphSet(const int& width, const int& height);
    ~GlyphSet();

    size_t GetCount() const;
    glm::ivec2 GetSize() const;

    void SetChar(const int& x, const int& y, const char& data);
    void SetData(const int& x, const int& y, const GlyphData& data);
    void StreamData(const int& pos, const int& len, const GlyphData* data);
    void SetSize(const int& width, const int& height);

    void Render() const;
    void Print() const;

    static void CleanUpVertices();

  private:
    static GLuint vertice_vbo_id_;
    static GLuint vertice_ebo_id_;

    static bool vertice_vbo_set_up_;
    static const glm::vec2 vertices_[4];
    static const GLuint elements_[6];

    GLuint vao_id_;
    GLuint instance_vbo_id_;

    int set_width_;
    int set_height_;

    std::vector<GlyphData> set_data_;
    std::string set_chars_;

    static void InitVertices();

    void InitData();
    void InitBuffers();
    void SetGLBuffer() const;
    void RecalculateSet(const int& new_width, const int& new_height);
  };
}

#endif
