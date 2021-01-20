/// @author James Holtom

#ifndef COMMON_UNIFORM_H
#define COMMON_UNIFORM_H

#include "../utility/GLUtils.h"

namespace term_engine::shaders {
  extern GLuint ubo_id;

  struct CommonUniformData {
    glm::mat4 projection;
  };

  void InitUBO();

  void CleanUpUBO();

  void ResetProjection();
}

#endif
