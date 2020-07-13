/*
 * Copyright 2020
 * James Holtom
 * MIT License
 */

#ifndef SHADER_MANAGER_H
#define SHADER_MANAGER_H

#include <initializer_list>
#include <memory>
#include <string>
#include <unordered_map>

#include "../gl_includes.h"
#include "Shader.h"

namespace term_engine::shaders {
  typedef std::shared_ptr<Shader> ShaderPtr;
  typedef std::pair<std::string, ShaderPtr> ShaderIter;
  typedef std::unordered_map<std::string, ShaderPtr> ShaderMap;

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
