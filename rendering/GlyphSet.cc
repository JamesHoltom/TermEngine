#include <algorithm>
#include <ostream>

#include "GlyphSet.h"
#include "../utility/DebugFunctions.h"
#include "../utility/spdlogUtils.h"
#include <spdlog/fmt/ostr.h>

namespace term_engine::glyphs {
  std::ostream& operator<< (std::ostream& os, const GlyphData& data) {
    return os << std::endl
      << "Vertex offsets:    " << data.vertex_offset.x << ", " << data.vertex_offset.y << std::endl
      << "Texture layer:     " << data.texture_offset << std::endl
      << "Foreground colour: " << data.foreground_color.r << ", " << data.foreground_color.g << ", " << data.foreground_color.b << ", " << data.foreground_color.a << std::endl
      << "Background colour: " << data.background_color.r << ", " << data.background_color.g << ", " << data.background_color.b << ", " << data.background_color.a << std::endl;
  }

  GlyphSet::GlyphSet(const std::string& font, const int& pt_size):
    is_dirty_(false),
    vao_id_(0),
    vbo_id_(0),
    set_width_(1),
    set_height_(1),
    glyph_scale_(glm::ivec2(0)),
    set_chars_(""),
    set_data_(std::make_unique<GlyphDataList>())
  {
    InitData();
    InitBuffers();
  }

  GlyphSet::~GlyphSet() {
    set_data_.reset();

    glDeleteVertexArrays(1, &vao_id_);
    glDeleteBuffers(1, &vbo_id_);
  }

  char GlyphSet::GetChar(const int& x, const int& y) const {
    if (x < 0 || x >= set_width_ || y < 0 || y >= set_height_) {
      spdlog::warn("Invalid coords {},{} given to GetChar.", x, y);

      return '\0';
    }

    size_t pos = (set_width_ * y) + x;

    return set_chars_.at(pos);
  }

  glm::ivec2 GlyphSet::GetScale() const {
    return glyph_scale_;
  }

  glm::ivec2 GlyphSet::GetPadding() const {
    return glyph_padding_;
  }

  glm::ivec2 GlyphSet::GetSpacing() const {
    return glyph_spacing_;
  }

  glm::ivec2 GlyphSet::GetSize() const {
    return glm::ivec2(set_width_, set_height_);
  }

  void GlyphSet::SetScale(const int& width, const int& height) {
    if (width <= 0 || height <= 0) {
      spdlog::warn("Invalid dimensions {}x{} given to SetScale.", width, height);

      return;
    }

    glyph_scale_ = glm::ivec2(width, height);
  }

  void GlyphSet::SetPadding(const int& x_padding, const int& y_padding) {
    glyph_padding_ = glm::ivec2(x_padding, y_padding);
  }

  void GlyphSet::SetSpacing(const int& x_spacing, const int& y_spacing) {
    glyph_spacing_ = glm::ivec2(x_spacing, y_spacing);
  }

  void GlyphSet::SetSize(const int& width, const int& height) {
    if (width * height > MAX_GLYPHS || width <= 0 || height <= 0) {
      spdlog::warn("Invalid dimensions {}x{} given to SetSize.", width, height);

      return;
    }

    /*int copy_width = std::min({ (int)set_width_, width });
    size_t old_size = set_width_ * set_height_;*/
    size_t new_size = (size_t)width * (size_t)height;

    /*std::string new_chars = std::string(new_size, ' ');
    GlyphDataPtr new_data = std::make_unique<GlyphDataList>();

    new_data->reserve(MAX_GLYPHS);
    new_data->resize(new_size);

    std::fill(new_data.get()->begin(), new_data.get()->end(), GlyphData());

    for (size_t i = 0, j = 0; i < old_size; i += set_width_) {
      new_chars.assign(set_chars_, i, copy_width);
      std::copy(set_data_->begin() + i, set_data_->begin() + i + copy_width, new_data->begin() + j);

      j += width;
    }

    set_chars_ = std::string(new_chars);
    set_data_ = std::move(new_data); */

    set_chars_ = std::string(new_size, ' ');
    set_data_->clear();
    set_data_->resize(new_size);

    ResetAllOffsets();

    set_width_ = width;
    set_height_ = height;
    is_dirty_ = true;
  }

  void GlyphSet::SetChar(const int& x, const int& y, const char& glyph, fonts::FontAtlasPtr& font_atlas) {
    if (x < 0 || x >= set_width_ || y < 0 || y >= set_height_) {
      spdlog::warn("Invalid coords {},{} given to SetChar.", x, y);

      return;
    }

    size_t pos = (set_width_ * (size_t)y) + (size_t)x;
    GLuint texture_offset = font_atlas->GetCharacter(glyph);
    
    set_chars_.at(pos) = glyph;
    set_data_->at(pos).texture_offset = (float)texture_offset;

    is_dirty_ = true;
  }

  void GlyphSet::SetOffset(const int& x, const int& y, const glm::vec2& offset) {
    if (x < 0 || x >= set_width_ || y < 0 || y >= set_height_) {
      spdlog::warn("Invalid coords {},{} given to SetOffset.", x, y);

      return;
    }

    size_t pos = (set_width_ * (size_t)y) + (size_t)x;
    
    set_data_->at(pos).vertex_offset = offset;
    
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

  size_t GlyphSet::GetCount() const {
    return set_width_ * set_height_;
  }

  void GlyphSet::ResetOffset(const int& x, const int& y) {
    if (x < 0 || x >= set_width_ || y < 0 || y >= set_height_) {
      spdlog::warn("Invalid coords {},{} given to ResetOffset.", x, y);

      return;
    }

    size_t pos = (set_width_ * (size_t)y) + (size_t)x;
    int x_offset;
    int y_offset;


  }

  void GlyphSet::ResetAllOffsets() {

  }

  void GlyphSet::Render() {
    if (is_dirty_) {
      SetGLBuffer();

      is_dirty_ = false;
    }

    glBindVertexArray(vao_id_);
    glDrawElementsInstanced(GL_TRIANGLES, VERTEX_COUNT, GL_UNSIGNED_INT, 0, static_cast<GLsizei>(set_width_ * set_height_));
  }

  void GlyphSet::InitData() {
    set_chars_ = std::string(set_width_ * set_height_, ' ');

    set_data_->reserve(MAX_GLYPHS);
    set_data_->resize(set_width_ * set_height_);
    std::fill(set_data_->begin(), set_data_->end(), GlyphData());
  }

  void GlyphSet::InitBuffers() {
    glGenVertexArrays(1, &vao_id_);
    glBindVertexArray(vao_id_);

    // Create the buffer for the glyph & vertex data.
    glGenBuffers(1, &vbo_id_);

    glBindVertexBuffer(0, vertex_vbo_id, 0, sizeof(VertexData));
    glBindVertexBuffer(1, vbo_id_, 0, sizeof(GlyphData));

    glVertexBindingDivisor(0, 0);
    glVertexBindingDivisor(1, 1);

    // Configure the vertex position attribute.
    glEnableVertexAttribArray(0);
    glVertexAttribFormat(0, 2, GL_FLOAT, GL_FALSE, offsetof(VertexData, vertex_position));
    glVertexAttribBinding(0, 0);

    // Configure the texture position attribute.
    glEnableVertexAttribArray(1);
    glVertexAttribFormat(1, 2, GL_FLOAT, GL_FALSE, offsetof(VertexData, texture_position));
    glVertexAttribBinding(1, 0);

    // Configure the vertex offset attribute.
    glEnableVertexAttribArray(2);
    glVertexAttribFormat(2, 2, GL_FLOAT, GL_FALSE, offsetof(GlyphData, vertex_offset));
    glVertexAttribBinding(2, 1);

    // Configure the texture offset attribute.
    glEnableVertexAttribArray(3);
    glVertexAttribFormat(3, 2, GL_FLOAT, GL_FALSE, offsetof(GlyphData, texture_offset));
    glVertexAttribBinding(3, 1);

    // Configure the foreground colour attribute.
    glEnableVertexAttribArray(4);
    glVertexAttribFormat(4, 4, GL_FLOAT, GL_FALSE, offsetof(GlyphData, foreground_color));
    glVertexAttribBinding(4, 1);

    // Configure the background colour attribute.
    glEnableVertexAttribArray(5);
    glVertexAttribFormat(5, 4, GL_FLOAT, GL_FALSE, offsetof(GlyphData, background_color));
    glVertexAttribBinding(5, 1);

    SetGLBuffer();

    glBindBuffer(GL_ARRAY_BUFFER, vertex_ebo_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertex_ebo_id);

    debug::LogVAOData();

    glBindVertexArray(0);
  }

  void GlyphSet::SetGLBuffer() const {
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GlyphData) * set_data_->size(), set_data_->data(), GL_DYNAMIC_DRAW);

    spdlog::debug("Logging instance VBO...");
    debug::LogVBOData();

    PrintData();
  }

  void GlyphSet::PrintData() const {
    spdlog::debug("Printing glyph data:");
    for (size_t i = 0; i < set_chars_.length(); i += set_width_) {
      spdlog::debug("\"{}\"", set_chars_.substr(i, set_width_));
    }

    int count = 0;

    for (size_t i = 0; i < set_data_->size(); i++) {
      GlyphData data = set_data_->at(i);

      spdlog::debug("Element #{}:{}", count, data);

      count++;
    }
  }

  void InitVertexData() {
    VertexData vertex_data[4] = {
      { glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f) },
      { glm::vec2(1.0f, 0.0f), glm::vec2(1.0f, 0.0f) },
      { glm::vec2(0.0f, 1.0f), glm::vec2(0.0f, 1.0f) },
      { glm::vec2(1.0f, 1.0f), glm::vec2(1.0f, 1.0f) }
    };

    GLuint indice_data[6] = {
      0, 1, 2,
      1, 2, 3
    };

    glGenBuffers(1, &vertex_vbo_id);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_vbo_id);
    glBufferStorage(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, 0);

    glGenBuffers(1, &vertex_ebo_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertex_ebo_id);
    glBufferStorage(GL_ELEMENT_ARRAY_BUFFER, sizeof(indice_data), indice_data, 0);

    spdlog::debug("Logging vertex VBO...");
    debug::LogVBOData();

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }

  void CleanUpVertexData() {
    glDeleteBuffers(1, &vertex_vbo_id);
    glDeleteBuffers(1, &vertex_ebo_id);
  }

  GLuint vertex_vbo_id = 0;
  GLuint vertex_ebo_id = 0;
}
