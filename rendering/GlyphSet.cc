#include <algorithm>

#include <spdlog/spdlog.h>

#include "GlyphSet.h"

#include "../utility/DebugFunctions.h"

namespace term_engine::glyphs {
  GlyphSet::GlyphSet(const std::string& font, const int& pt_size, const int& width, const int& height):
    is_dirty_(false),
    vao_id_(0),
    instance_vbo_id_(0),
    set_width_(width),
    set_height_(height),
    set_chars_(std::make_unique<GlyphCharList>()),
    set_data_(std::make_unique<GlyphDataList>())
  {
    InitData();
    InitBuffers();
  }

  GlyphSet::~GlyphSet() {
    set_chars_.reset();
    set_data_.reset();

    glDeleteVertexArrays(1, &vao_id_);
    glDeleteBuffers(1, &instance_vbo_id_);
  }

  char GlyphSet::GetChar(const int& x, const int& y) const {
    if (x < 0 || x >= set_width_ || y < 0 || y >= set_height_) {
      spdlog::warn("Invalid coords {},{} given to GetChar.", x, y);

      return '\0';
    }

    size_t pos = (set_width_ * y) + x;

    return set_chars_->at(pos);
  }

  GlyphData GlyphSet::GetData(const int& x, const int& y) const {
    if (x < 0 || x >= set_width_ || y < 0 || y >= set_height_) {
      spdlog::warn("Invalid coords {},{} given to GetData.", x, y);

      return GlyphData();
    }

    size_t pos = (set_width_ * y) + x;

    return set_data_->at(pos);
  }

  glm::ivec2 GlyphSet::GetSize() const {
    return glm::ivec2(set_width_, set_height_);
  }

  size_t GlyphSet::GetCount() const {
    return set_width_ * set_height_;
  }

  void GlyphSet::SetChar(const int& x, const int& y, const char& glyph, fonts::FontAtlasPtr& font_atlas) {
    if (x < 0 || x >= set_width_ || y < 0 || y >= set_height_) {
      spdlog::warn("Invalid coords {},{} given to SetChar.", x, y);

      return;
    }

    size_t pos = (set_width_ * (size_t)y) + (size_t)x;
    
    set_chars_->at(pos) = glyph;
    set_data_->at(pos).texture_bounds = font_atlas->GetCharacter(glyph);

    is_dirty_ = true;
  }

  void GlyphSet::SetOffset(const int& x, const int& y, const glm::vec2& offset) {
    if (x < 0 || x >= set_width_ || y < 0 || y >= set_height_) {
      spdlog::warn("Invalid coords {},{} given to SetOffset.", x, y);

      return;
    }

    size_t pos = (set_width_ * (size_t)y) + (size_t)x;
    
    set_data_->at(pos).instance_offset = offset;
    
    is_dirty_ = true;
  }

  void GlyphSet::SetScale(const int& x, const int& y, const glm::vec2& scale) {
    if (x < 0 || x >= set_width_ || y < 0 || y >= set_height_) {
      spdlog::warn("Invalid coords {},{} given to SetScale.", x, y);

      return;
    }

    size_t pos = (set_width_ * (size_t)y) + (size_t)x;
    
    set_data_->at(pos).instance_scale = scale;
    
    is_dirty_ = true;
  }

  void GlyphSet::SetFGColor(const int& x, const int& y, const glm::vec4& fg_color) {
    if (x < 0 || x >= set_width_ || y < 0 || y >= set_height_) {
      spdlog::warn("Invalid coords {},{} given to SetFGColor.", x, y);

      return;
    }

    size_t pos = (set_width_ * (size_t)y) + (size_t)x;
    
    set_data_->at(pos).foreground_color = fg_color;
    
    is_dirty_ = true;
  }

  void GlyphSet::SetBGColor(const int& x, const int& y, const glm::vec4& bg_color) {
    if (x < 0 || x >= set_width_ || y < 0 || y >= set_height_) {
      spdlog::warn("Invalid coords {},{} given to SetBGColor.", x, y);

      return;
    }

    size_t pos = (set_width_ * (size_t)y) + (size_t)x;
    
    set_data_->at(pos).background_color = bg_color;
    
    is_dirty_ = true;
  }

  void GlyphSet::SetData(const int& x, const int& y, const GlyphData& data) {
    if (x < 0 || x >= set_width_ || y < 0 || y >= set_height_) {
      spdlog::warn("Invalid coords {},{} given to SetData.", x, y);

      return;
    }

    size_t pos = (set_width_ * (size_t)y) + (size_t)x;

    GlyphData &set_data = set_data_->at(pos);

    set_data.instance_offset = data.instance_offset;
    set_data.instance_scale = data.instance_scale;
    set_data.foreground_color = data.foreground_color;
    set_data.background_color = data.background_color;

    is_dirty_ = true;
  }

  void GlyphSet::SetSize(const int& width, const int& height) {
    if (width * height > MAX_GLYPHS || width <= 0 || height <= 0) {
      spdlog::warn("Invalid dimensions {}x{} given to SetSize.", width, height);

      return;
    }

    int copy_width = std::min({ (int)set_width_, width });
    size_t size = set_width_ * set_height_;

    GlyphCharPtr new_chars = GlyphCharPtr();
    GlyphDataPtr new_data = GlyphDataPtr();

    new_chars->resize(size);
    new_data->resize(size);

    std::fill(new_chars.get()->begin(), new_chars.get()->end(), ' ');
    std::fill(new_data.get()->begin(), new_data.get()->end(), GlyphData());

    for (size_t i = 0, j = 0; i < size; i += set_width_) {
      std::copy(set_chars_->begin() + i, set_chars_->begin() + i + copy_width, new_chars->begin() + j);
      std::copy(set_data_->begin() + i, set_data_->begin() + i + copy_width, new_data->begin() + j);

      j += width;
    }

    set_chars_ = std::move(new_chars);
    set_data_ = std::move(new_data);

    set_width_ = width;
    set_height_ = height;
    is_dirty_ = true;
  }

  void GlyphSet::Render() {
    if (is_dirty_) {
      SetGLBuffer();

      is_dirty_ = false;
    }

    glBindVertexArray(vao_id_);
    glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr, static_cast<GLsizei>(set_width_ * set_height_));
  }

  void GlyphSet::InitData() {
    set_chars_->reserve(MAX_GLYPHS);
    set_data_->reserve(MAX_GLYPHS);

    set_chars_->resize(set_width_ * set_height_);
    set_data_->resize(set_width_ * set_height_);
  }

  void GlyphSet::InitBuffers() {
    glGenVertexArrays(1, &vao_id_);
    glBindVertexArray(vao_id_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertice_ebo_id);

    // Set up the VAO and the buffer for the vertice data.
    glBindBuffer(GL_ARRAY_BUFFER, vertice_vbo_id);

    debug::LogVBOData();

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // Create the buffer for the glyph data.
    glGenBuffers(1, &instance_vbo_id_);

    SetGLBuffer();

    glBindBuffer(GL_ARRAY_BUFFER, instance_vbo_id_);

    // Configure the position attribute.
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);

    // Configure the scale attribute.
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)(sizeof(glm::vec2)));

    // Configure the glyph attribute.
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)(2 * sizeof(glm::vec2)));

    // Configure the foreground colour attribute.
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)((2 * sizeof(glm::vec2)) + sizeof(glm::vec4)));

    // Configure the background colour attribute.
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)((2 * sizeof(glm::vec2) + sizeof(glm::vec4))));

    glVertexAttribDivisor(1, 1);
    glVertexAttribDivisor(2, 1);
    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertice_ebo_id);

    debug::LogVAOData();

    glBindVertexArray(0);
  }

  void GlyphSet::SetGLBuffer() const {
    glBindBuffer(GL_ARRAY_BUFFER, instance_vbo_id_);

    glBufferData(GL_ARRAY_BUFFER, sizeof(GlyphData) * set_data_->size(), set_data_->data(), GL_DYNAMIC_DRAW);

    debug::LogVBOData();

    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }

  void InitVertices() {
    glGenBuffers(1, &vertice_vbo_id);
    glGenBuffers(1, &vertice_ebo_id);

    glBindBuffer(GL_ARRAY_BUFFER, vertice_vbo_id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertice_ebo_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), &elements[0], GL_STATIC_DRAW);
  }

  void CleanUpVertices() {
    glDeleteBuffers(1, &vertice_vbo_id);
    glDeleteBuffers(1, &vertice_ebo_id);
  }

  GLuint vertice_vbo_id = 0;
  GLuint vertice_ebo_id = 0;

  const glm::vec2 vertices[4] = {
      glm::vec2(-1.0f, 1.0f),  // Top-left corner
      glm::vec2(1.0f, 1.0f),   // Top-right corner
      glm::vec2(-1.0f, -1.0f), // Bottom-left corner
      glm::vec2(1.0f, -1.0f)   // Bottom-right corner
  };
  const GLuint elements[6] = {
      0, 1, 2,
      2, 1, 3
  };
}
