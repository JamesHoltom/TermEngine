#include <string>
#include "GLUtils.h"
#include "ImGuiUtils.h"
#include "SDLUtils.h"
#include "LogUtils.h"

namespace term_engine::utility {
  void GLAPIENTRY glDebugOutput(uint32_t source, uint32_t type, uint32_t id, uint32_t severity, int, const char* message, const void*)
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

    LogError("GL debug message (#{}):\nDescription: {}\nSource: {}\nType: {}\nSeverity: {}", id, message, source_string, type_string, severity_string);
  }

  void InitGL()
  {
    LogDebug("Requesting OpenGL version {}.{}", MAJOR_VERSION, MINOR_VERSION);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, MAJOR_VERSION);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, MINOR_VERSION);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);

    int major_ver, minor_ver;
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &major_ver);
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &minor_ver);

    LogDebug("Running OpenGL version {}.{}", major_ver, minor_ver);
    LogDebug("Initialised OpenGL.");
  }

  bool InitContext(SDL_Window* window)
  {
    context = SDL_GL_CreateContext(window);

    if (context == nullptr)
    {
      utility::LogError("An error occurred whilst creating the context: {}", SDL_GetError());

      return false;
    }

    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK) {
      LogError("Failed to initialise GLEW!");

      return false;
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glEnable(GL_PROGRAM_POINT_SIZE);
    glDebugMessageCallback(glDebugOutput, 0);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

    LogDebug("Initialised GLEW.");

    return true;
  }

  void CleanUpContext()
  {
    if (context != NULL)
    {
      SDL_GL_DeleteContext(&context);
    }
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
        LogDebug("Program build results:\nID: {}\nMessage: {}", program_id, info_log);
      }
      else
      {
        LogDebug("Program build results:\nID: {}\nNo message to display.", program_id);
      }

      delete[] info_log;
    }
    else
    {
      LogWarn("Program with an ID of {} has not been built.", program_id);
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
        LogDebug("Shader build results:\nID: {}, Type: {}\nMessage: {}", shader_id, GetShaderTypeName(shader_type), info_log);
      }
      else
      {
        LogDebug("Shader build results:\nID: {}, Type: {}\nNo message to display.", shader_id, GetShaderTypeName(shader_type));
      }

      delete[] info_log;
    }
    else
    {
      LogWarn("Shader with an ID of {} has not been built. Error #{}", shader_id, glGetError());
    }
  }

  ShaderProcessResult LinkShaderProgram(uint32_t program_id)
  {
    int program_linked = GL_FALSE;

    glLinkProgram(program_id);
    glGetProgramiv(program_id, GL_LINK_STATUS, &program_linked);

    PrintProgramLog(program_id);

    if (program_linked == GL_TRUE)
    {
      LogDebug("Linked program with ID {}", program_id);
    }
    else
    {
      LogError("Failed to link shader program for ID {}.", program_id);
    }

    return ShaderProcessResult(program_id, program_linked);
  }

  ShaderProcessResult CompileShaderStage(const std::string& glsl_code, uint32_t type)
  {
    int shader_compiled = GL_FALSE;
    uint32_t shader_id = glCreateShader(type);
    const char* source_c_string = { glsl_code.c_str() };

    if (shader_id == 0)
    {
      utility::LogError("Failed to create shader stage! Error #{}", glGetError());
    }

    glShaderSource(shader_id, 1, &source_c_string, nullptr);
    glCompileShader(shader_id);
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &shader_compiled);

    PrintStageLog(shader_id);

    if (shader_compiled == GL_TRUE)
    {
      LogDebug("Compiled {} shader stage with ID {}", GetShaderTypeName(type), shader_id);
    }
    else
    {
      LogError("Failed to compile GLSL shader for ID {}.", shader_id);
    }

    return ShaderProcessResult(shader_id, shader_compiled);
  }

  UniformList GetUniforms(uint32_t program_id)
  {
    int count, max_len;
    UniformList uniforms;

    glGetProgramiv(program_id, GL_ACTIVE_UNIFORMS, &count);
    glGetProgramiv(program_id, GL_ACTIVE_UNIFORM_MAX_LENGTH, &max_len);

    uniforms.reserve(count);

    for (int i = 0; i < count; ++i)
    {
      Uniform uniform;
      char* uniform_name = new char[max_len];
      int name_len;

      glGetActiveUniform(program_id, i, max_len, &name_len, &uniform.count_, &uniform.type_, uniform_name);

      uniform.location_ = i;
      uniform.name_ = std::string(uniform_name, name_len);

      delete[] uniform_name;

      uniforms.push_back(uniform);

      utility::LogDebug("Found uniform #{} for program {} with name: \"{}\", count: {}, type: {}", uniform.location_, program_id, uniform.name_, uniform.count_, uniform.type_);
    }

    return uniforms;
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

  std::string GetUniformTypeName(uint32_t type)
  {
    std::string result = "unknown";

    switch (type)
    {
      case GL_FLOAT: result = "float"; break;
      case GL_INT: result = "int"; break;
      case GL_UNSIGNED_INT: result = "uint"; break;
      case GL_FLOAT_VEC2: result = "vec2"; break;
      case GL_FLOAT_VEC3: result = "vec3"; break;
      case GL_FLOAT_VEC4: result = "vec4"; break;
      case GL_INT_VEC2: result = "ivec2"; break;
      case GL_INT_VEC3: result = "ivec3"; break;
      case GL_INT_VEC4: result = "ivec4"; break;
      case GL_UNSIGNED_INT_VEC2: result = "uvec2"; break;
      case GL_UNSIGNED_INT_VEC3: result = "uvec3"; break;
      case GL_UNSIGNED_INT_VEC4: result = "uvec4"; break;
      case GL_FLOAT_MAT2: result = "mat2"; break;
      case GL_FLOAT_MAT3: result = "mat3"; break;
      case GL_FLOAT_MAT4: result = "mat4"; break;
      case GL_FLOAT_MAT2x3: result = "mat2x3"; break;
      case GL_FLOAT_MAT2x4: result = "mat2x4"; break;
      case GL_FLOAT_MAT3x2: result = "mat3x2"; break;
      case GL_FLOAT_MAT3x4: result = "mat3x4"; break;
      case GL_FLOAT_MAT4x2: result = "mat4x2"; break;
      case GL_FLOAT_MAT4x3: result = "mat4x3"; break;
      case GL_SAMPLER_1D: result = "sampler1D"; break;
      case GL_SAMPLER_2D: result = "sampler2D"; break;
      case GL_SAMPLER_3D: result = "sampler3D"; break;
    }

    return result;
  }
}
