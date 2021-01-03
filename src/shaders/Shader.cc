#include <fstream>
#include <sstream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "../logging/Logger.h"

namespace term_engine::shaders {
  Shader::Shader(const std::string& name):
    name_(name),
    program_id_(0),
    shader_id_list_({}) {}

  Shader::~Shader() {
    RemoveProgram();
    RemoveShaders();
  }

  std::string Shader::GetName() const {
    return name_;
  }

  GLuint Shader::GetProgramID() const {
    return program_id_;
  }

  bool Shader::BuildShaderFromString(const GLchar* source, const GLenum& type) {
    GLint shader_compiled = GL_FALSE;
    GLuint shader_id = glCreateShader(type);

    glShaderSource(shader_id, 1, &source, nullptr);
    glCompileShader(shader_id);
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &shader_compiled);

    if (shader_compiled == GL_TRUE) {
      shader_id_list_.push_back(shader_id);

      logging::logger->debug("Generated GLSL shader for ID {}.", shader_id);
    }
    else {
      logging::logger->error("Failed to compile GLSL shader for ID {}.", shader_id);
    }

    PrintShaderLog(shader_id);

    return shader_compiled;
  }

  bool Shader::BuildShaderFromFile(const std::string& file_name, const GLenum& type) {
    std::ifstream shader_file;
    std::stringstream shader_path;
    std::stringstream shader_source;

    shader_file.exceptions(std::ifstream::badbit | std::ifstream::failbit);
    shader_path << "shaders/" << file_name;

    try {
      shader_file.open(shader_path.str());

      shader_source << shader_file.rdbuf();

      shader_file.close();
    }
    catch (std::ifstream::failure exception) {
      logging::logger->error("Failed to read GLSL shader from file \'{}\'.", file_name);
    }

    return BuildShaderFromString({ shader_source.str().c_str() }, type);
  }

  bool Shader::BuildProgram() {
    GLint program_linked = GL_FALSE;
    GLuint program_id = glCreateProgram();

    for (GLuint shader : shader_id_list_) {
      glAttachShader(program_id, shader);
    }

    glLinkProgram(program_id);
    glGetProgramiv(program_id, GL_LINK_STATUS, &program_linked);

    if (program_linked == GL_TRUE) {
      program_id_ = program_id;

      RemoveShaders();

      logging::logger->debug("Generated GLSL shader program {}.", program_id_);
    }
    else {
      logging::logger->error("Failed to link GLSL shader program {}.", program_id);
    }

    PrintProgramLog();

    return program_linked;
  }

  bool Shader::IsBuilt() const {
    return program_id_ > 0;
  }

  void Shader::Use() const {
    glUseProgram(program_id_);
  }

  void Shader::Unuse() const {
    glUseProgram(0);
  }

  void Shader::SetUniformInt(const std::string& name, const GLuint& count, const GLint* data) {
    GLint uniform_id = glGetUniformLocation(program_id_, name.c_str());

    switch (count) {
    case 1:
      glUniform1i(uniform_id, data[0]);
      break;
    case 2:
      glUniform2i(uniform_id, data[0], data[1]);
      break;
    case 3:
      glUniform3i(uniform_id, data[0], data[1], data[2]);
      break;
    case 4:
      glUniform4i(uniform_id, data[0], data[1], data[2], data[3]);
      break;
    }
  }

  void Shader::SetUniformUint(const std::string& name, const GLuint& count, const GLuint* data) {
    GLint uniform_id = glGetUniformLocation(program_id_, name.c_str());

    switch (count) {
    case 1:
      glUniform1ui(uniform_id, data[0]);
      break;
    case 2:
      glUniform2ui(uniform_id, data[0], data[1]);
      break;
    case 3:
      glUniform3ui(uniform_id, data[0], data[1], data[2]);
      break;
    case 4:
      glUniform4ui(uniform_id, data[0], data[1], data[2], data[3]);
      break;
    }
  }

  void Shader::SetUniformFloat(const std::string& name, const GLuint& count, const GLfloat* data) {
    GLint uniform_id = glGetUniformLocation(program_id_, name.c_str());

    switch (count) {
    case 1:
      glUniform1f(uniform_id, data[0]);
      break;
    case 2:
      glUniform2f(uniform_id, data[0], data[1]);
      break;
    case 3:
      glUniform3f(uniform_id, data[0], data[1], data[2]);
      break;
    case 4:
      glUniform4f(uniform_id, data[0], data[1], data[2], data[3]);
      break;
    }
  }

  void Shader::SetUniformMatrix(const std::string& name, const glm::uvec2& dimensions, const GLfloat* data) {
    GLint uniform_id = glGetUniformLocation(program_id_, name.c_str());

    switch (dimensions.x) {
    case 2:
      switch (dimensions.y) {
      case 2:
        glUniformMatrix2fv(uniform_id, 1, GL_FALSE, data);
        break;
      case 3:
        glUniformMatrix2x3fv(uniform_id, 1, GL_FALSE, data);
        break;
      case 4:
        glUniformMatrix2x4fv(uniform_id, 1, GL_FALSE, data);
        break;
      }

      break;
    case 3:
      switch (dimensions.y) {
      case 2:
        glUniformMatrix3x2fv(uniform_id, 1, GL_FALSE, data);
        break;
      case 3:
        glUniformMatrix3fv(uniform_id, 1, GL_FALSE, data);
        break;
      case 4:
        glUniformMatrix3x4fv(uniform_id, 1, GL_FALSE, data);
        break;
      }

      break;
    case 4:
      switch (dimensions.y) {
      case 2:
        glUniformMatrix4x2fv(uniform_id, 1, GL_FALSE, data);
        break;
      case 3:
        glUniformMatrix4x3fv(uniform_id, 1, GL_FALSE, data);
        break;
      case 4:
        glUniformMatrix4fv(uniform_id, 1, GL_FALSE, data);
        break;
      }

      break;
    }
  }

  void Shader::PrintProgramLog() {
    if (glIsProgram(program_id_)) {
      int log_length;
      int max_log_length;

      glGetProgramiv(program_id_, GL_INFO_LOG_LENGTH, &max_log_length);

      GLchar* info_log = new char[max_log_length];

      glGetProgramInfoLog(program_id_, max_log_length, &log_length, info_log);

      if (log_length > 0) {
        logging::logger->debug("Program build results:\nID: {}\nMessage: {}", program_id_, info_log);
      }
      else {
        logging::logger->debug("Program build results:\nID: {}\nNo message to display.", program_id_);
      }

      delete[] info_log;
    }
    else {
      logging::logger->warn("Program with an ID of {} has not been built.", program_id_);
    }
  }

  void Shader::PrintShaderLog(const GLuint& shader_id) {
    if (glIsShader(shader_id)) {
      int log_length;
      int max_log_length;

      glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &max_log_length);

      GLchar* info_log = new char[max_log_length];

      glGetShaderInfoLog(shader_id, max_log_length, &log_length, info_log);

      if (log_length > 0) {
        logging::logger->debug("Shader build results:\nID: {}\nMessage: {}", shader_id, info_log);
      }
      else {
        logging::logger->debug("Shader build results:\nID: {}\nNo message to display.", shader_id);
      }

      delete[] info_log;
    }
    else {
      logging::logger->warn("Shader with an ID of {} has not been built.", shader_id);
    }
  }

  void Shader::RemoveProgram() {
    glDeleteProgram(program_id_);
  }

  void Shader::RemoveShaders() {
    for (GLuint shader : shader_id_list_) {
      glDeleteShader(shader);
    }

    shader_id_list_.clear();
  }
}
