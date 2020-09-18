/*
 * Copyright 2020
 * James Holtom
 * MIT License
 */

#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

#include "../gl_includes.h"

namespace term_engine::shaders {
  typedef std::vector<GLuint> ShaderList;
  typedef std::unordered_map<std::string, GLuint> UniformMap;

  class Shader {
  public:

    Shader(const std::string& name);
    ~Shader();

    std::string GetName() const;
    GLuint GetProgramID() const;

    bool BuildShaderFromString(const GLchar* source, const GLenum& type);
    bool BuildShaderFromFile(const std::string& file_name, const GLenum& type);

    bool BuildProgram();
    bool IsBuilt() const;

    void Use() const;
    void Unuse() const;

    GLuint GetUniformID(const std::string& uniform_name);

    void SetUniformInt(const std::string& name, const int& count, const int* data);

  private:

    std::string name_;

    GLuint program_id_;
    ShaderList shader_id_list_;
    UniformMap uniform_id_list_;

    void PrintProgramLog();
    void PrintShaderLog(const GLuint& shader_id);

    void RemoveProgram();
    void RemoveShaders();
  };
}

#endif
