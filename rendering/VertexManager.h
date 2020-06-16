/*
 * Copyright 2020
 * James Holtom
 * MIT License
 */

#ifndef VERTEX_MANAGER_H
#define VERTEX_MANAGER_H

#include "../gl_includes.h"

namespace term_engine {
  void CreateVertexData();

  extern GLuint vertex_array_object_id_;
  extern GLuint vertex_buffer_object_id_;
  extern GLuint element_buffer_object_id_;
  extern GLuint position_attribute_location_;
}

#endif
