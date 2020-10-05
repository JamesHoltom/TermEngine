#include "Window.h"
#include "../init.h"
#include "../utility/spdlogUtils.h"

namespace term_engine::windows {
  void Init() {
    window = SDL::Window(SDL_CreateWindow("TermEngine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, DEFAULT_WIDTH, DEFAULT_HEIGHT, SDL_WINDOW_OPENGL));

    if (window == nullptr) {
      spdlog::error("An SDL error occurred: {}", SDL_GetError());

      return;
    }

    context = SDL_GL_CreateContext(window.get());

    // GLEW needs to be initialised as soon as a GL context is created.
    if (context == NULL) {
      spdlog::error("An SDL error occurred: {}", SDL_GetError());

      return;
    }

    InitGLEW();

    SDL_GL_MakeCurrent(window.get(), context);

    spdlog::debug("Created window.");
  }

  void CleanUp() {
    SDL_GL_DeleteContext(context);
    window.reset();

    spdlog::debug("Destroyed window.");
  }

  SDL_GLContext context;
  SDL::Window window;
}