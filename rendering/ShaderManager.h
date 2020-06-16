/*
 * Copyright 2020
 * James Holtom
 * MIT License
 */

#ifndef SHADER_MANAGER_H
#define SHADER_MANAGER_H

#include <initializer_list>
#include <string>
#include <tuple>
#include <unordered_map>

#include "../gl_includes.h"

namespace term_engine::shaders {
  typedef std::tuple<GLint, GLuint> BuildResult;
  typedef std::unordered_map<std::string, GLuint> ShaderMap;
  
  BuildResult BuildShaderFromString(const GLchar* source, const GLenum& type);
  BuildResult BuildShaderFromFile(const std::string& file_name, const GLenum& type);

  BuildResult BuildProgram(const std::string& name, std::initializer_list<GLuint> shaders);

  GLuint GetProgram(const std::string& name);

  void PrintProgramLog(const GLuint& program_id);
  void PrintShaderLog(const GLuint& shader_id);

  extern const GLchar* default_fragment_shader;
  extern const GLchar* default_vertex_shader;

  extern ShaderMap shader_list;
}

#endif
