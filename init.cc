#include <stdlib.h>

#include "init.h"
#include "utility/FTUtils.h"
#include "utility/GLUtils.h"
#include "utility/spdlogUtils.h"

namespace term_engine {
  int InitSDL() {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
      spdlog::error("Failed to initialise SDL!\nError: {}", SDL_GetError());

      return 1;
    }

    srand(SDL_GetTicks());

    spdlog::debug("Initialised SDL.");

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

    spdlog::debug("Running OpenGL version {}.{}", major_ver, minor_ver);
    spdlog::debug("Initialised OpenGL.");

    return 0;
  }

  int InitGLEW() {
    glewExperimental = GL_TRUE;

    GLenum glew_status = glewInit();

    if (glew_status == GLEW_OK) {
      spdlog::debug("Initialised GLEW.");
    }
    else {
      spdlog::error("Failed to initialise GLEW!");

      return 1;
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(GL::glDebugOutput, 0);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

    return 0;
  }

  int InitFreeType(FT_Library *library) {
    FT_Error err = FT::Log(FT_Init_FreeType(library));

    if (err != FT_Err_Ok) {
      spdlog::error("Failed to initialise FreeType. Received error #{}", err);

      return 1;
    }

    spdlog::debug("Initialised FreeType.");

    return 0;
  }

  void CleanUpSDL() {
    SDL_Quit();

    spdlog::debug("Shut down SDL.");
  }

  void CleanUpFreeType(const FT_Library& library) {
    FT::Log(FT_Done_FreeType(library));

    spdlog::debug("Shut down FreeType.");
  }
}
