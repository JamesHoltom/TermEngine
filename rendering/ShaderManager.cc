#include <fstream>
#include <sstream>
#include <spdlog/spdlog.h>

#include "ShaderManager.h"
#include "../gl_includes.h"

namespace term_engine::shaders {
  BuildResult BuildShaderFromString(const GLchar* source, const GLenum& type) {
    GLint shader_compiled = GL_FALSE;
    GLuint shader_id = glCreateShader(type);

    glShaderSource(shader_id, 1, &source, nullptr);
    glCompileShader(shader_id);
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &shader_compiled);

    if (shader_compiled == GL_TRUE) {
      spdlog::info("Generated GLSL shader {}.", shader_id);
    }
    else {
      spdlog::error("Failed to compile GLSL shader for ID {}!", shader_id);
    }

    PrintShaderLog(shader_id);

    return BuildResult{ shader_compiled, shader_id };
  }

  BuildResult BuildShaderFromFile(const std::string& file_name, const GLenum& type) {
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
      spdlog::error("Failed to read GLSL shader from file {}!", file_name);
    }

    return BuildShaderFromString({ shader_source.str().c_str() }, type);
  }

  BuildResult BuildProgram(const std::string& name, std::initializer_list<GLuint> shaders) {
    GLint program_linked = GL_FALSE;
    GLuint program_id = glCreateProgram();

    for (auto shader : shaders) {
      glAttachShader(program_id, shader);
    }

    glLinkProgram(program_id);
    glGetProgramiv(program_id, GL_LINK_STATUS, &program_linked);

    if (program_linked == GL_TRUE) {
      shader_list.insert(std::make_pair(name, program_id));

      for (auto shader : shaders) {
        glDeleteShader(shader);
      }

      spdlog::info("Generated GLSL shader program {}.", program_id);
    }
    else {
      spdlog::error("Failed to link GLSL shader program {}!", program_id);
    }

    PrintProgramLog(program_id);

    return BuildResult{ program_linked, program_id };
  }

  GLuint GetProgram(const std::string& name) {
    return shader_list.at(name);
  }

  void PrintProgramLog(const GLuint& program_id) {
    if (glIsProgram(program_id)) {
      int log_length;
      int max_log_length;

      glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &max_log_length);

      GLchar* info_log = new char[max_log_length];

      glGetProgramInfoLog(program_id, max_log_length, &log_length, info_log);

      if (log_length > 0) {
        spdlog::info("Program build results:\nID: {}\nMessage: {}", program_id, info_log);
      }
      else {
        spdlog::info("Program build results:\nID: {}\nNo message to display.", program_id);
      }

      delete[] info_log;
    }
    else {
      spdlog::warn("There is no program with an ID of {}.", program_id);
    }
  }

  void PrintShaderLog(const GLuint& shader_id) {
    if (glIsShader(shader_id)) {
      int log_length;
      int max_log_length;

      glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &max_log_length);

      GLchar* info_log = new char[max_log_length];

      glGetShaderInfoLog(shader_id, max_log_length, &log_length, info_log);

      if (log_length > 0) {
        spdlog::info("Shader build results:\nID: {}\nMessage: {}", shader_id, info_log);
      }
      else {
        spdlog::info("Shader build results:\nID: {}\nNo message to display.", shader_id);
      }

      delete[] info_log;
    }
    else {
      spdlog::warn("There is no shader with an ID of {}.", shader_id);
    }
  }

  const GLchar* default_fragment_shader = {
    "#version 330 core\n"
    "out vec4 fragment_color;\n"
    "void main() {\n"
    "\tfragment_color = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n"
    "}"
  };

  const GLchar* default_vertex_shader = {
    "#version 330 core\n"
    "layout(location = 0) in vec2 position;\n"
    "void main() {\n"
    "\tgl_Position = vec4(position.x, position.y, 0.0f, 1.0f);\n"
    "}"
  };

  ShaderMap shader_list;
}
