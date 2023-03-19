#include <string>
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#include "GLUtils.h"
#include "SDLUtils.h"
#include "SpdlogUtils.h"

namespace term_engine::utility {
  void GLAPIENTRY glDebugOutput(uint32_t source, uint32_t type, uint32_t id, uint32_t severity, int length, const char* message, const void* userParam)
  {
    // Ignore non-significant error/warning codes
    if (id == 131169 || id == 131185 || id == 131218 || id == 131204) {
      return;
    }

    std::string source_string;
    std::string type_string;
    std::string severity_string;

    switch (source) {
    case GL_DEBUG_SOURCE_API:             source_string = "API";             break;
    case GL_DEBUG_SOURCE_APPLICATION:     source_string = "Application";     break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER: source_string = "Shader Compiler"; break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:     source_string = "Third Party";     break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   source_string = "Window System";   break;
    case GL_DEBUG_SOURCE_OTHER:           source_string = "Other";           break;
    }

    switch (type) {
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: type_string = "Deprecated Behavior"; break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  type_string = "Undefined Behavior";  break;
    case GL_DEBUG_TYPE_ERROR:               type_string = "Error";               break;
    case GL_DEBUG_TYPE_MARKER:              type_string = "Marker";              break;
    case GL_DEBUG_TYPE_PERFORMANCE:         type_string = "Performance";         break;
    case GL_DEBUG_TYPE_POP_GROUP:           type_string = "Pop Group";           break;
    case GL_DEBUG_TYPE_PUSH_GROUP:          type_string = "Push Group";          break;
    case GL_DEBUG_TYPE_PORTABILITY:         type_string = "Portability";         break;
    case GL_DEBUG_TYPE_OTHER:               type_string = "Other";               break;
    }

    switch (severity) {
    case GL_DEBUG_SEVERITY_HIGH:         severity_string = "High";         break;
    case GL_DEBUG_SEVERITY_MEDIUM:       severity_string = "Medium";       break;
    case GL_DEBUG_SEVERITY_LOW:          severity_string = "Low";          break;
    case GL_DEBUG_SEVERITY_NOTIFICATION: severity_string = "Notification"; break;
    }

    logger->error("GL debug message (#{}):\nDescription: {}\nSource: {}\nType: {}\nSeverity: {}", id, message, source_string, type_string, severity_string);
  }

  void InitGL()
  {
    logger->debug("Requesting OpenGL version {}.{}", MAJOR_VERSION, MINOR_VERSION);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, MAJOR_VERSION);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, MINOR_VERSION);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);

    int major_ver, minor_ver;
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &major_ver);
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &minor_ver);

    utility::logger->debug("Running OpenGL version {}.{}", major_ver, minor_ver);
    utility::logger->debug("Initialised OpenGL.");
  }

  bool InitGLEW()
  {
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK) {
      utility::logger->error("Failed to initialise GLEW!");

      return false;
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(glDebugOutput, 0);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

    utility::logger->debug("Initialised GLEW.");

    return true;
  }

  void PrintProgramLog(uint32_t program_id)
  {
    if (glIsProgram(program_id))
    {
      int log_length;
      int max_log_length;

      glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &max_log_length);

      char* info_log = new char[max_log_length];

      glGetProgramInfoLog(program_id, max_log_length, &log_length, info_log);

      if (log_length > 0)
      {
        logger->debug("Program build results:\nID: {}\nMessage: {}", program_id, info_log);
      }
      else
      {
        logger->debug("Program build results:\nID: {}\nNo message to display.", program_id);
      }

      delete[] info_log;
    }
    else
    {
      logger->warn("Program with an ID of {} has not been built.", program_id);
    }
  }

  void PrintStageLog(uint32_t shader_id)
  {
    if (glIsShader(shader_id))
    {
      int log_length;
      int max_log_length;
      int shader_type;

      glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &max_log_length);
      glGetShaderiv(shader_id, GL_SHADER_TYPE, &shader_type);

      char* info_log = new char[max_log_length];

      glGetShaderInfoLog(shader_id, max_log_length, &log_length, info_log);

      if (log_length > 0)
      {
        logger->debug("Shader build results:\nID: {}, Type: {}\nMessage: {}", shader_id, GetShaderTypeName(shader_type), info_log);
      }
      else
      {
        logger->debug("Shader build results:\nID: {}, Type: {}\nNo message to display.", shader_id, GetShaderTypeName(shader_type));
      }

      delete[] info_log;
    }
    else
    {
      logger->warn("Shader with an ID of {} has not been built.", shader_id);
    }
  }

  ShaderProcessResult LinkShaderProgram(uint32_t program_id)
  {
    int program_linked = GL_FALSE;

    glLinkProgram(program_id);
    glGetProgramiv(program_id, GL_LINK_STATUS, &program_linked);

    PrintProgramLog(program_id);

    if (program_linked == GL_FALSE)
    {
      logger->error("Failed to link shader program for ID {}.", program_id);
    }

    return ShaderProcessResult(program_id, program_linked);
  }

  ShaderProcessResult CompileShaderStage(const std::string& glsl_code, uint32_t type)
  {
    int shader_compiled = GL_FALSE;
    uint32_t shader_id = glCreateShader(type);
    const char* source_c_string = { glsl_code.c_str() };

    glShaderSource(shader_id, 1, &source_c_string, nullptr);
    glCompileShader(shader_id);
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &shader_compiled);

    PrintStageLog(shader_id);

    if (shader_compiled == GL_FALSE)
    {
      logger->error("Failed to compile GLSL shader for ID {}.", shader_id);
    }

    return ShaderProcessResult(shader_id, shader_compiled);
  }

  std::string GetShaderTypeName(uint32_t type)
  {
    std::string result = "unknown";

    switch (type)
    {
      case GL_FRAGMENT_SHADER:  result = "fragment";  break;
      case GL_VERTEX_SHADER:    result = "vertex";    break;
      case GL_GEOMETRY_SHADER:  result = "geometry";  break;
    }

    return result;
  }
}
