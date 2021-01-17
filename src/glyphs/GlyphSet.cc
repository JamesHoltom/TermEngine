#include <algorithm>
#include <sstream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "GlyphSet.h"
#include "../fonts/FontAtlasManager.h"
#include "../shaders/ShaderManager.h"
#include "../logging/Logger.h"
#include "../window/Window.h"
#include "../utility/DebugFunctions.h"

namespace term_engine::glyphs {

  GLuint vertex_vbo_id = 0;
  GLuint vertex_ebo_id = 0;

  GlyphSet::GlyphSet(const fonts::FontAtlasPtr& font_atlas, const shaders::ShaderPtr& shader):
    is_dirty_(false),
    vao_id_(0),
    vbo_id_(0),
    glyph_padding_(glm::vec2(0.0f)),
    glyph_spacing_(glm::vec2(0.0f)),
    set_position_(glm::vec2(0.0f)),
    set_size_(glm::uvec2(DEFAULT_SET_WIDTH, DEFAULT_SET_HEIGHT)),
    set_font_(font_atlas),
    set_shader_(shader)
  {
    InitData();
    InitBuffers();

    logging::logger->info("GS: There are {} font refs.", set_font_.use_count());
    logging::logger->info("GS: There are {} shader refs.", set_shader_.use_count());

    const GLint active_texture = 0;
    set_shader_->Use();
    set_shader_->SetUniformInt("font_texture", 1, &active_texture);
    set_shader_->Unuse();

    SetPosition(glm::vec2(0.0f));
    ResetAllScale();
    ResetProjection();
  }

  GlyphSet::~GlyphSet()
  {
    glDeleteVertexArrays(1, &vao_id_);
    glDeleteBuffers(1, &vbo_id_);

    set_font_.reset();
    set_shader_.reset();

    logging::logger->info("~GS: There are {} font refs.", set_font_.use_count());
    logging::logger->info("~GS: There are {} shader refs.", set_shader_.use_count());
  }

  glm::vec2 GlyphSet::GetPosition() const
  {
    return set_position_;
  }

  glm::vec2 GlyphSet::GetPadding() const
  {
    return glyph_padding_;
  }

  glm::vec2 GlyphSet::GetSpacing() const
  {
    return glyph_spacing_;
  }

  glm::uvec2 GlyphSet::GetSize() const
  {
    return set_size_;
  }

  GlyphList& GlyphSet::GetData()
  {
    return set_data_;
  }

  std::string GlyphSet::GetFontPath() const
  {
    return set_font_->GetFontPath();
  }

  void GlyphSet::SetPosition(const glm::vec2& position)
  {
    set_position_ = position;

    set_shader_->Use();
    set_shader_->SetUniformFloat("origin", 2, glm::value_ptr(set_position_));
    set_shader_->Unuse();
  }

  void GlyphSet::SetPadding(const glm::vec2& padding)
  {
    glyph_padding_ = padding;
    is_dirty_ = true;
  }

  void GlyphSet::SetSpacing(const glm::vec2& spacing)
  {
    glyph_spacing_ = spacing;
    is_dirty_ = true;
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

  void GlyphSet::SetFont(const std::string& name)
  {
    set_font_ = fonts::GetFontAtlas(name);
    is_dirty_ = true;
  }

  void GlyphSet::SetFont(const fonts::FontAtlasPtr& font)
  {
    set_font_ = font;
    is_dirty_ = true;
  }

  void GlyphSet::SetShader(const std::string& name)
  {
    set_shader_ = shaders::GetShader(name);
  }

  void GlyphSet::SetShader(const shaders::ShaderPtr& shader)
  {
    set_shader_ = shader;
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

  void GlyphSet::ResetAllOffsets()
  {
    for (GlyphData& glyph : set_data_) {
      glyph.offset_ = glm::vec2(0.0f);
    }

    is_dirty_ = true;
  }

  void GlyphSet::SetAllScale(const glm::vec2& scale)
  {
    if (scale.x <= 0 || scale.y <= 0) {
      logging::logger->warn("Invalid dimensions {}x{} given to SetAllScale.", scale);

      return;
    }

    for (GlyphData& glyph : set_data_) {
      glyph.scale_ = scale;
    }

    is_dirty_ = true;
  }

  void GlyphSet::ResetAllScale()
  {
    SetAllScale(glm::vec2(set_font_->GetTextureSize()));
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

  void GlyphSet::ResetProjection()
  {
    const glm::ivec2 window_size = windows::GetWindowSize();
    const glm::mat4 projection = glm::ortho(0.0f, (GLfloat)window_size.x, (GLfloat)window_size.y, 0.0f);

    glViewport(0, 0, window_size.x, window_size.y);

    set_shader_->Use();
    set_shader_->SetUniformMatrix("projection", glm::ivec2(4, 4), glm::value_ptr(projection));
    set_shader_->Unuse();
  }

  bool GlyphSet::IsDirty() const
  {
    return is_dirty_;
  }

  void GlyphSet::Dirty()
  {
    is_dirty_ = true;
  }

  void GlyphSet::Render()
  {
    if (is_dirty_) {
      SetGLBuffer();

      is_dirty_ = false;
    }

    set_shader_->Use();
    set_font_->Use();

    glBindVertexArray(vao_id_);
    glDrawArrays(GL_POINTS, 0, static_cast<GLsizei>(GetCount()));

    set_font_->Unuse();
    set_shader_->Unuse();
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

    // Configure the glyph offset attribute.
    glEnableVertexAttribArray(1);
    glVertexAttribFormat(1, 2, GL_FLOAT, GL_FALSE, offsetof(BufferData, offset_));
    glVertexAttribBinding(1, 0);
    
    // Configure the glyph scale attribute.
    glEnableVertexAttribArray(2);
    glVertexAttribFormat(2, 2, GL_FLOAT, GL_FALSE, offsetof(BufferData, scale_));
    glVertexAttribBinding(2, 0);

    // Configure the foreground colour attribute.
    glEnableVertexAttribArray(3);
    glVertexAttribFormat(3, 4, GL_FLOAT, GL_FALSE, offsetof(BufferData, foreground_color_));
    glVertexAttribBinding(3, 0);

    // Configure the background colour attribute.
    glEnableVertexAttribArray(4);
    glVertexAttribFormat(4, 4, GL_FLOAT, GL_FALSE, offsetof(BufferData, background_color_));
    glVertexAttribBinding(4, 0);

    debug::LogVAOData();

    glBindVertexArray(0);
  }

  void GlyphSet::SetGLBuffer() const
  {
    BufferList buffer_data;
    buffer_data.reserve(set_data_.capacity());

    for (const GlyphData& glyph : set_data_) {
      BufferData data((GLfloat)set_font_->GetCharacter(glyph.character_), GetGlyphOrigin(glyph), glyph.scale_ + (glyph_padding_ * 2.0f), glyph.foreground_color_, glyph.background_color_);

      buffer_data.push_back(data);
    }

    glBindVertexArray(vao_id_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(BufferData) * buffer_data.size(), buffer_data.data(), GL_DYNAMIC_DRAW);

    logging::logger->debug("Logging instance VBO...");
    debug::LogVBOData();

    PrintData();
    PrintBuffer(buffer_data);
  }

  glm::vec2 GlyphSet::GetGlyphOrigin(const GlyphData& glyph) const
  {
    return glm::vec2(glyph.index_) * (glyph.scale_ + glyph_spacing_ + (glyph_padding_ * 2.0f)) + glyph.offset_;
  }

  void GlyphSet::PrintBuffer(const BufferList& data) const
  {
    logging::logger->debug("Printing buffer data:");

    unsigned int count = 0;

    for (const BufferData& item : data) {
      logging::logger->debug("Item #{}:{}", count, item);

      ++count;
    }
  }
}
