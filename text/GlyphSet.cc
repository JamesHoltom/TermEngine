#include <sstream>

#include <spdlog/spdlog.h>

#include "GlyphSet.h"

namespace term_engine::glyphs {
  GlyphSet::GlyphSet() :
    vao_id_(0),
    instance_vbo_id_(0),
    set_width_(0),
    set_height_(0)
  {
    InitData();
    InitBuffers();
  }

  GlyphSet::GlyphSet(const int& width, const int& height):
    vao_id_(0),
    instance_vbo_id_(0),
    set_width_(width),
    set_height_(height)
  {
    InitData();
    InitBuffers();
  }

  GlyphSet::~GlyphSet() {
    glDeleteVertexArrays(1, &vao_id_);
    glDeleteBuffers(1, &instance_vbo_id_);
  }

  size_t GlyphSet::GetCount() const {
    return (size_t)(set_width_ * set_height_);
  }

  glm::ivec2 GlyphSet::GetSize() const {
    return glm::ivec2(set_width_, set_height_);
  }

  void GlyphSet::SetChar(const int& x, const int& y, const char& data) {
    if (x < 0 || x >= set_width_ || y < 0 || y >= set_height_) {
      spdlog::warn("Invalid coords {},{} given to SetChar.", x, y);

      return;
    }

    int pos = (x * y) + x;

    set_chars_.at(pos) = data;
  }

  void GlyphSet::SetData(const int& x, const int& y, const GlyphData& data) {
    if (x < 0 || x >= set_width_ || y < 0 || y >= set_height_) {
      spdlog::warn("Invalid coords {},{} given to SetData.", x, y);

      return;
    }

    int pos = (set_width_ * y) + x;

    set_data_.at(pos) = data;

    glBindBuffer(GL_ARRAY_BUFFER, instance_vbo_id_);
    glBufferSubData(GL_ARRAY_BUFFER, pos * sizeof(GlyphData), sizeof(GlyphData), &data);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }

  void GlyphSet::StreamData(const int& pos, const int& len, const GlyphData* data) {
    if ((size_t)(pos + len) > set_data_.size()) {
      spdlog::warn("Incoming data to StreamData is OOB.");

      return;
    }

    for (int i = pos, j = 0; i < pos + len; i++) {
      set_data_.at(i) = data[j++];
    }

    glBindBuffer(GL_ARRAY_BUFFER, instance_vbo_id_);
    glBufferSubData(GL_ARRAY_BUFFER, pos * sizeof(GlyphData), len * sizeof(GlyphData), data);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }

  void GlyphSet::SetSize(const int& width, const int& height) {
    if (width * height > MAX_GLYPHS || width <= 0 || height <= 0) {
      spdlog::warn("Invalid dimensions {}x{} given to SetSize.", width, height);

      return;
    }

    RecalculateSet(width, height);
    SetGLBuffer();

    set_width_ = width;
    set_height_ = height;
  }

  void GlyphSet::Render() const {
    glBindVertexArray(vao_id_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertice_ebo_id_);
    glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, static_cast<GLsizei>(set_data_.size()));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
  }

  void GlyphSet::Print() const {
    std::stringstream char_string;

    for (int pos = 0; pos >= set_chars_.length(); pos += set_width_) {
      char_string << set_chars_.substr(pos, set_width_) << "\n";
    }

    spdlog::info("Glyph set: {}", char_string.str());

    glBindBuffer(GL_ARRAY_BUFFER, instance_vbo_id_);
    GlyphData* data = (GlyphData*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_ONLY);

    glUnmapBuffer(GL_ARRAY_BUFFER);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }

  void GlyphSet::InitVertices() {
    glGenBuffers(1, &GlyphSet::vertice_vbo_id_);
    glGenBuffers(1, &GlyphSet::vertice_ebo_id_);

    glBindBuffer(GL_ARRAY_BUFFER, GlyphSet::vertice_vbo_id_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GlyphSet::vertices_), &GlyphSet::vertices_[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GlyphSet::vertice_ebo_id_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GlyphSet::elements_), &GlyphSet::elements_[0], GL_STATIC_DRAW);
  }

  void GlyphSet::CleanUpVertices() {
    glDeleteBuffers(1, &GlyphSet::vertice_vbo_id_);
    glDeleteBuffers(1, &GlyphSet::vertice_ebo_id_);
  }

  void GlyphSet::InitData() {
    set_data_.reserve(MAX_GLYPHS);
    set_chars_.reserve(MAX_GLYPHS);

    int size = set_width_ * set_height_;

    if (set_width_ >= 0 && set_height_ >= 0 && size <= MAX_GLYPHS) {
      set_data_.resize(size);
      set_chars_.resize(size);
    }
  }

  void GlyphSet::InitBuffers() {
    glGenVertexArrays(1, &vao_id_);
    glBindVertexArray(vao_id_);

    // Create the buffer for the position data, if this is the first time.
    if (!vertice_vbo_set_up_) {
      InitVertices();
    }

    // Set up the position data.
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, GlyphSet::vertice_vbo_id_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GlyphSet::vertice_ebo_id_);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // Create the buffer for the offset, scale & color data.
    glGenBuffers(1, &instance_vbo_id_);
    SetGLBuffer();

    // Set up the offset, scale & color data.
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, instance_vbo_id_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GlyphData), (void*)0);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GlyphData), (void*)sizeof(glm::vec2));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(GlyphData), (void*)(2 * sizeof(glm::vec2)));

    glVertexAttribDivisor(1, 1);
    glVertexAttribDivisor(2, 1);
    glVertexAttribDivisor(3, 1);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  }

  void GlyphSet::SetGLBuffer() const {
    glBindBuffer(GL_ARRAY_BUFFER, instance_vbo_id_);

    if (set_data_.size() > 0) {
      glBufferData(GL_ARRAY_BUFFER, set_data_.size() * sizeof(GlyphData), &set_data_.front(), GL_STATIC_DRAW);
    }
    else {
      glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_STATIC_DRAW);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }

  void GlyphSet::RecalculateSet(const int& new_width, const int& new_height) {
    if (set_width_ == new_width && set_height_ == new_height) {
      return;
    }

    unsigned int offset = std::min({ set_width_, new_width });
    unsigned int length = set_width_ * std::min({ set_height_, new_height });

    std::string new_set;
    new_set.resize((size_t)(new_width * new_height), ' ');

    for (unsigned int pos = 0; pos >= length; pos += set_width_) {
      new_set.assign(set_chars_, pos, offset);
    }

    set_chars_ = new_set;
  }

  GLuint GlyphSet::vertice_vbo_id_ = 0;
  GLuint GlyphSet::vertice_ebo_id_ = 0;

  bool GlyphSet::vertice_vbo_set_up_ = false;
  const glm::vec2 GlyphSet::vertices_[4] = {
      glm::vec2(-1.0f, 1.0f),  // Top-left corner
      glm::vec2(1.0f, 1.0f),   // Top-right corner
      glm::vec2(-1.0f, -1.0f), // Bottom-left corner
      glm::vec2(1.0f, -1.0f)   // Bottom-right corner
  };
  const GLuint GlyphSet::elements_[6] = {
      0, 1, 2,
      2, 1, 3
  };
}
