#include <algorithm>
#include <sstream>

#include "GlyphSet.h"
#include "../fonts/FontAtlasManager.h"
#include "../logging/Logger.h"
#include "../shaders/ShaderManager.h"
#include "../utility/DebugFunctions.h"

namespace term_engine::glyphs {
  GlyphSet::GlyphSet():
    is_dirty_(false),
    vao_id_(0),
    vbo_id_(0),
    set_size_(glm::uvec2(DEFAULT_SET_WIDTH, DEFAULT_SET_HEIGHT))
  {
    InitData();
    InitBuffers();
  }

  GlyphSet::~GlyphSet()
  {
    glDeleteVertexArrays(1, &vao_id_);
    glDeleteBuffers(1, &vbo_id_);
  }

  glm::uvec2 GlyphSet::GetSize() const
  {
    return set_size_;
  }

  GlyphList& GlyphSet::GetData()
  {
    return set_data_;
  }

  // TODO: Fix code to keep contents after resize.
  void GlyphSet::SetSize(const glm::uvec2& size)
  {
    if (size.x * size.y > MAX_GLYPHS || size.x <= 0 || size.y <= 0) {
      logging::logger->warn("Invalid dimensions {}x{} given to SetSize.", size);

      return;
    }

    /*int copy_width = std::min({ (int)set_width_, width });
    size_t old_size = set_width_ * set_height_;*/
    size_t new_size = (size_t)size.x * (size_t)size.y;

    /*std::string new_chars = std::string(new_size, ' ');
    GlyphListPtr new_data = std::make_unique<GlyphDataList>();

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

    set_size_ = size;
    is_dirty_ = true;

    set_data_.clear();

    for (int index = 0; index < GetCount(); ++index) {
      int x = index % size.x;
      int y = index / size.x;

      GlyphData data(glm::uvec2(x, y));

      set_data_.push_back(data);
    }

    logging::logger->debug("Resized to {} set with {} elements.", set_size_, GetCount());
  }

  size_t GlyphSet::GetCount() const
  {
    return (size_t)set_size_.x * (size_t)set_size_.y;
  }

  GetGlyphResult GlyphSet::GetGlyph(const glm::uvec2& index)
  {
    glm::uvec2 clamped_index = index;

    if (index.x < 0 || index.x >= set_size_.x || index.y < 0 || index.y >= set_size_.y) {
      logging::logger->warn("Invalid index {} given to GetGlyph.", index);
      
      return std::make_pair(false, set_data_.at(0));
    }

    size_t pos = ((size_t)set_size_.x * (size_t)clamped_index.y) + (size_t)clamped_index.x;

    return std::make_pair(true, set_data_.at(pos));
  }

  void GlyphSet::ClearAll()
  {
    for (GlyphData& glyph : set_data_) {
      glyph.Clear();
    }

    is_dirty_ = true;
  }

  void GlyphSet::ResetAllPositions()
  {
    for (GlyphData& glyph : set_data_) {
      glyph.offset_ = glm::vec2(0.0f);
    }

    is_dirty_ = true;
  }

  void GlyphSet::PrintData() const
  {
    logging::logger->debug("Printing glyph data:");

    unsigned int count = 0;
    std::stringstream set_line;
    set_line << "Character map:";

    for (const GlyphData& glyph : set_data_) {
      logging::logger->debug("Glyph #{}:{}", count, glyph);

      if (count % set_size_.x == 0) {
        set_line << std::endl;
      }

      set_line << glyph.character_;

      count++;
    }

    logging::logger->debug(set_line.str());
  }

  bool GlyphSet::IsDirty() const
  {
    return is_dirty_;
  }

  void GlyphSet::Dirty()
  {
    is_dirty_ = true;
  }

  void GlyphSet::Render(const fonts::FontAtlasPtr& font, const shaders::ShaderPtr& shader)
  {
    if (is_dirty_) {
      SetGLBuffer(font);

      is_dirty_ = false;
    }

    shader->Use();
    font->Use();

    glBindVertexArray(vao_id_);

    glDrawArrays(GL_POINTS, 0, static_cast<GLsizei>(GetCount()));

    font->Unuse();
    shader->Unuse();
  }

  void GlyphSet::InitData()
  {
    set_data_.reserve(MAX_GLYPHS);
    
    int count = 0;

    while (count < GetCount()) {
      int x = count % set_size_.x;
      int y = count / set_size_.x;

      set_data_.push_back(GlyphData(glm::uvec2(x, y)));

      count++;
    }
  }

  void GlyphSet::InitBuffers()
  {
    glGenVertexArrays(1, &vao_id_);
    glBindVertexArray(vao_id_);

    glGenBuffers(1, &vbo_id_);
    glBindVertexBuffer(0, vbo_id_, 0, sizeof(BufferData));

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

    buffer_data_.reserve(MAX_GLYPHS);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(BufferData) * MAX_GLYPHS, buffer_data_.data(), GL_DYNAMIC_DRAW);

    glBindVertexArray(0);
  }

  void GlyphSet::SetGLBuffer(const fonts::FontAtlasPtr& font)
  {
    buffer_data_.clear();

    for (const GlyphData& glyph : set_data_) {
      glm::vec2 position = glm::vec2(glyph.index_) * font->GetTextureSize();

      BufferData data((GLfloat)font->GetCharacter(glyph.character_), position, glyph.foreground_color_, glyph.background_color_);

      buffer_data_.push_back(data);
    }

    glBindVertexArray(vao_id_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id_);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(BufferData) * GetCount(), buffer_data_.data());

    logging::logger->debug("Logging instance VBO...");
    debug::LogVBOData();

    PrintData();
    PrintBuffer();
  }

  void GlyphSet::PrintBuffer() const
  {
    logging::logger->debug("Printing buffer data:");

    unsigned int count = 0;

    for (const BufferData& item : buffer_data_) {
      logging::logger->debug("Item #{}:{}", count, item);

      ++count;
    }
  }
}
