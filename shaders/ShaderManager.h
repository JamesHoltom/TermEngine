/*
 * Copyright 2020
 * James Holtom
 * MIT License
 */

#ifndef SHADER_MANAGER_H
#define SHADER_MANAGER_H

#include <initializer_list>
#include <string>

#include "Shader.h"
#include "../utility/GLUtils.h"

namespace term_engine::shaders {
  void InitGlyphShader();

  ShaderPtr GetShader(const std::string& name);
  ShaderPtr AddShader(const std::string& name);
  void RemoveShader(const std::string& name);

  void CleanUpShaders();

  extern ShaderMap shader_list;

  extern const GLchar* glyph_fragment_shader;
  extern const GLchar* glyph_vertex_shader;
}

#endif
