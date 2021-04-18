#include "Uniform.h"
#include "../system/Window.h"

namespace term_engine::data {
  GLuint ubo_id;
  UniformData ubo_data;

  void Init()
  {
    glGenBuffers(1, &ubo_id);
    glBindBuffer(GL_UNIFORM_BUFFER, ubo_id);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(UniformData), nullptr, GL_STATIC_DRAW);

    glBindBufferBase(GL_UNIFORM_BUFFER, 0, ubo_id);
  }

  void CleanUp()
  {
    glDeleteBuffers(1, &ubo_id);
  }

  void SetProjection(const glm::ivec2& window_size)
  {
    const glm::mat4 projection = glm::ortho(0.0f, (GLfloat)window_size.x, (GLfloat)window_size.y, 0.0f);

    glBindBuffer(GL_UNIFORM_BUFFER, ubo_id);
    glBufferSubData(GL_UNIFORM_BUFFER, offsetof(UniformData, projection_), sizeof(glm::mat4), glm::value_ptr(projection));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glViewport(0, 0, (GLsizei)window_size.x, (GLsizei)window_size.y);
  }

  glm::vec2 GetFontSize()
  {
    return ubo_data.font_size_;
  }

  void SetFontSize(const glm::vec2& size)
  {
    ubo_data.font_size_ = size;

    glBindBuffer(GL_UNIFORM_BUFFER, ubo_id);
    glBufferSubData(GL_UNIFORM_BUFFER, offsetof(UniformData, font_size_), sizeof(glm::vec2), glm::value_ptr(ubo_data.font_size_));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
  }

  glm::vec2 GetPosition()
  {
    return ubo_data.set_position_;
  }

  glm::vec2 GetPadding()
  {
    return ubo_data.set_padding_;
  }

  glm::vec2 GetSpacing()
  {
    return ubo_data.set_spacing_;
  }

  void SetPosition(const glm::vec2& position)
  {
    ubo_data.set_position_ = position;

    glBindBuffer(GL_UNIFORM_BUFFER, ubo_id);
    glBufferSubData(GL_UNIFORM_BUFFER, offsetof(UniformData, set_position_), sizeof(glm::vec2), glm::value_ptr(ubo_data.set_position_));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
  }

  void SetPadding(const glm::vec2& padding)
  {
    ubo_data.set_padding_ = padding;

    glBindBuffer(GL_UNIFORM_BUFFER, ubo_id);
    glBufferSubData(GL_UNIFORM_BUFFER, offsetof(UniformData, set_padding_), sizeof(glm::vec2), glm::value_ptr(ubo_data.set_padding_));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
  }

  void SetSpacing(const glm::vec2& spacing)
  {
    ubo_data.set_spacing_ = spacing;

    glBindBuffer(GL_UNIFORM_BUFFER, ubo_id);
    glBufferSubData(GL_UNIFORM_BUFFER, offsetof(UniformData, set_spacing_), sizeof(glm::vec2), glm::value_ptr(ubo_data.set_spacing_));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
  }
}