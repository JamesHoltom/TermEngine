#include "Uniform.h"
#include "../resources/Window.h"

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

  glm::vec2 GetPosition()
  {
    return ubo_data.set_position_;
  }

  void SetPosition(const glm::vec2& position)
  {
    ubo_data.set_position_ = position;

    glBindBuffer(GL_UNIFORM_BUFFER, ubo_id);
    glBufferSubData(GL_UNIFORM_BUFFER, offsetof(UniformData, set_position_), sizeof(glm::vec2), glm::value_ptr(ubo_data.set_position_));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
  }
}