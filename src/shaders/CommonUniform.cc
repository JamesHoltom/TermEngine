#include "CommonUniform.h"
#include "../window/Window.h"

namespace term_engine::shaders {
  GLuint ubo_id;

  void InitUBO()
  {
    glGenBuffers(1, &ubo_id);
    glBindBuffer(GL_UNIFORM_BUFFER, ubo_id);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(CommonUniformData), nullptr, GL_STATIC_DRAW);

    ResetProjection();

    glBindBufferBase(GL_UNIFORM_BUFFER, 0, ubo_id);
  }

  void CleanUpUBO()
  {
    glDeleteBuffers(1, &ubo_id);
  }

  void ResetProjection()
  {
    const glm::ivec2 window_size = windows::GetWindowSize();
    const glm::mat4 projection = glm::ortho(0.0f, (GLfloat)window_size.x, (GLfloat)window_size.y, 0.0f);

    glBindBuffer(GL_UNIFORM_BUFFER, ubo_id);
    glBufferSubData(GL_UNIFORM_BUFFER, offsetof(CommonUniformData, projection), sizeof(projection), glm::value_ptr(projection));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glViewport(0, 0, window_size.x, window_size.y);
  }
}