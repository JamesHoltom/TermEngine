#include <stdlib.h>
#include <spdlog/spdlog.h>

#include "init.h"

namespace term_engine {
  void GLAPIENTRY glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const char* message, const void* userParam) {
    // ignore non-significant error/warning codes
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
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: type_string = "Undefined Behavior"; break;
    case GL_DEBUG_TYPE_ERROR: type_string = "Error"; break;
    case GL_DEBUG_TYPE_MARKER: type_string = "Marker"; break;
    case GL_DEBUG_TYPE_PERFORMANCE: type_string = "Performance"; break;
    case GL_DEBUG_TYPE_POP_GROUP: type_string = "Pop Group"; break;
    case GL_DEBUG_TYPE_PUSH_GROUP: type_string = "Push Group"; break;
    case GL_DEBUG_TYPE_PORTABILITY: type_string = "Portability"; break;
    case GL_DEBUG_TYPE_OTHER: type_string = "Other"; break;
    }

    switch (severity) {
    case GL_DEBUG_SEVERITY_HIGH:         severity_string = "High";         break;
    case GL_DEBUG_SEVERITY_MEDIUM:       severity_string = "Medium";       break;
    case GL_DEBUG_SEVERITY_LOW:          severity_string = "Low";          break;
    case GL_DEBUG_SEVERITY_NOTIFICATION: severity_string = "Notification"; break;
    }

    spdlog::error("GL debug message (#{}): {}\nSource: {}\nType: {}\n, Severity: {}", id, message, source_string, type_string, severity_string);
  }

  int InitSDL() {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
      spdlog::error("Failed to initialise SDL!\nError: {}", SDL_GetError());

      return 1;
    }

    srand(SDL_GetTicks());

    spdlog::info("Initialised SDL.");

    return 0;
  }

  int InitGL() {
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);

    int major_ver, minor_ver;
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &major_ver);
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &minor_ver);

    spdlog::info("Running OpenGL version {}.{}", major_ver, minor_ver);

    glEnable(GL_DEPTH_TEST);

    spdlog::info("Initialised OpenGL.");

    return 0;
  }

  int InitGLEW() {
    glewExperimental = GL_TRUE;

    GLenum glew_status = glewInit();

    if (glew_status == GLEW_OK) {
      spdlog::info("Initialised GLEW.");
    }
    else {
      spdlog::error("Failed to initialise GLEW!");
    }

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(glDebugOutput, 0);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

    return 0;
  }

  void Shutdown() {
    SDL_Quit();

    spdlog::info("Shut down libraries.");
  }
}
