#include "Window.h"
#include "../init.h"
#include "../logging/Logger.h"

namespace term_engine::windows {
  void Init() {
    window = SDL::Window(SDL_CreateWindow("TermEngine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, DEFAULT_WIDTH, DEFAULT_HEIGHT, SDL_WINDOW_OPENGL));

    if (window == nullptr) {
      logging::logger->error("An SDL error occurred: {}", SDL_GetError());

      return;
    }

    context = SDL_GL_CreateContext(window.get());

    // GLEW needs to be initialised as soon as a GL context is created.
    if (context == NULL) {
      logging::logger->error("An SDL error occurred: {}", SDL_GetError());

      return;
    }

    InitGLEW();

    SDL_GL_MakeCurrent(window.get(), context);

    logging::logger->debug("Created window.");
  }

  void CleanUp() {
    SDL_GL_DeleteContext(context);
    window.reset();

    logging::logger->debug("Destroyed window.");
  }

  glm::ivec2 GetWindowSize() {
    glm::ivec2 size = glm::ivec2(0);
    SDL_GetWindowSize(window.get(), &size.x, &size.y);

    return size;
  }

  void SetWindowSize(const glm::ivec2& size) {
    SDL_SetWindowSize(window.get(), size.x, size.y);
  }

  SDL_GLContext context;
  SDL::Window window;
}