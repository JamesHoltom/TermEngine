#include <string>
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

#include "GLUtils.h"
#include "../logging/Logger.h"

namespace GL {
  void GLAPIENTRY glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const char* message, const void* userParam)
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

    term_engine::logging::logger->error("GL debug message (#{}):\nDescription: {}\nSource: {}\nType: {}\nSeverity: {}", id, message, source_string, type_string, severity_string);
  }
}
