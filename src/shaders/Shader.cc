#include <fstream>
#include <sstream>

#include "Shader.h"
#include "../logging/Logger.h"
#include "../system/FileFunctions.h"

namespace term_engine::shaders {
  GLuint CreateProgram()
  {
    return glCreateProgram();
  }

  GLint AddShaderString(const GLuint& program_id, const GLenum& type, const std::string& shader)
  {
    logging::logger->debug("Compiling shader stage...");

    GLint shader_compiled = GL_FALSE;
    GLuint shader_id = glCreateShader(type);
    const GLchar* source_c_string = { shader.c_str() };

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

      return GL_FALSE;
    }
  }

  GLint AddShaderFile(const GLuint& program_id, const GLenum& type, const std::string& file)
  {
    const std::string source_string = system::ReadFile(file);

    return AddShaderString(program_id, type, source_string);
  }

  GLint LinkProgram(const GLuint& program_id)
  {
    GLint program_linked = GL_FALSE;

    glLinkProgram(program_id);
    glGetProgramiv(program_id, GL_LINK_STATUS, &program_linked);

    PrintProgramLog(program_id);

    if (program_linked == GL_TRUE) {
      logging::logger->debug("Generated GLSL shader program {}.", program_id);

      return GL_TRUE;
    }
    else {
      logging::logger->error("Failed to link GLSL shader program {}.", program_id);

      return GL_FALSE;
    }
  }

  void PrintProgramLog(const GLuint& program_id)
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
}
