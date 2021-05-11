#include <fstream>
#include <sstream>

#include "Shader.h"
#include "../logging/Logger.h"
#include "../system/FileFunctions.h"

namespace term_engine::shaders {
  int Init()
  {
    GLint program_linked = GL_FALSE;
    program_id = glCreateProgram();

    GLuint vs_id = AddShaderFile(std::string(GLSL_VERTEX_FILE), GL_VERTEX_SHADER);
    GLuint gs_id = AddShaderFile(std::string(GLSL_GEOMETRY_FILE), GL_GEOMETRY_SHADER);
    GLuint fs_id = AddShaderFile(std::string(GLSL_FRAGMENT_FILE), GL_FRAGMENT_SHADER);

    glLinkProgram(program_id);
    glGetProgramiv(program_id, GL_LINK_STATUS, &program_linked);

    PrintProgramLog();

    if (program_linked == GL_TRUE) {
      glDeleteShader(vs_id);
      glDeleteShader(gs_id);
      glDeleteShader(fs_id);

      glUseProgram(program_id);

      logging::logger->debug("Generated GLSL shader program {}.", program_id);
    }
    else {
      logging::logger->error("Failed to link GLSL shader program {}.", program_id);

      return 1;
    }

    return 0;
  }

  void CleanUp()
  {
    glUseProgram(0);
    glDeleteProgram(program_id);
  }

  GLuint AddShaderFile(const std::string& filename, const GLenum& type)
  {
    GLint shader_compiled = GL_FALSE;
    GLuint shader_id = glCreateShader(type);

    const std::string source_string = system::ReadFromFile(filename);
    const GLchar* source_c_string = { source_string.c_str() };

    glShaderSource(shader_id, 1, &source_c_string, nullptr);
    glCompileShader(shader_id);
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &shader_compiled);

    PrintStageLog(shader_id);

    if (shader_compiled == GL_TRUE) {
      glAttachShader(program_id, shader_id);

      logging::logger->debug("Generated GLSL shader for ID {}.", shader_id);

      return shader_id;
    }
    else {
      logging::logger->error("Failed to compile GLSL shader for ID {}.", shader_id);

      return 0;
    }
  }

  void PrintProgramLog()
  {
    if (glIsProgram(program_id)) {
      int log_length;
      int max_log_length;

      glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &max_log_length);

      GLchar* info_log = new char[max_log_length];

      glGetProgramInfoLog(program_id, max_log_length, &log_length, info_log);

      if (log_length > 0) {
        logging::logger->debug("Program build results:\nID: {}\nMessage: {}", program_id, info_log);
      }
      else {
        logging::logger->debug("Program build results:\nID: {}\nNo message to display.", program_id);
      }

      delete[] info_log;
    }
    else {
      logging::logger->warn("Program with an ID of {} has not been built.", program_id);
    }
  }

  void PrintStageLog(const GLuint& shader_id)
  {
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

  GLuint program_id;
}
