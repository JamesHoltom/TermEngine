/*
 * Copyright 2020
 * James Holtom
 * MIT License
 */

#ifndef SHADER_H
#define SHADER_H

#include <memory>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

#include <glm/glm.hpp>

#include "../utility/GLUtils.h"

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

    void SetUniformInt(const std::string& name, const GLuint& count, const GLint* data);
    void SetUniformUint(const std::string& name, const GLuint& count, const GLuint* data);
    void SetUniformFloat(const std::string& name, const GLuint& count, const GLfloat* data);
    void SetUniformMatrix(const std::string& name, const glm::uvec2& dimensions, const GLfloat* data);

  private:
    std::string name_;

    GLuint program_id_;
    ShaderList shader_id_list_;

    void PrintProgramLog();
    void PrintShaderLog(const GLuint& shader_id);

    void RemoveProgram();
    void RemoveShaders();
  };

  typedef std::shared_ptr<Shader> ShaderPtr;
  typedef std::pair<std::string, ShaderPtr> ShaderIter;
  typedef std::unordered_map<std::string, ShaderPtr> ShaderMap;
}

#endif // ! SHADER_H
