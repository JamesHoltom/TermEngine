#include "Window.h"
#include "../data/Uniform.h"
#include "../resources/FontAtlas.h"
#include "../logging/Logger.h"
#include "../view/View.h"

namespace term_engine::system {
  SDL_GLContext context;
  SDL::Window window;
  GLfloat window_colour_r, window_colour_g, window_colour_b;
  GLuint window_render_mode;

  int InitWindow()
  {
    window = SDL::Window(SDL_CreateWindow("TermEngine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, DEFAULT_WIDTH, DEFAULT_HEIGHT, SDL_WINDOW_OPENGL));

    if (window == nullptr) {
      logging::logger->error("An SDL error occurred: {}", SDL_GetError());

      return 1;
    }

    context = SDL_GL_CreateContext(window.get());

    // GLEW needs to be initialised as soon as a GL context is created.
    if (context == NULL) {
      logging::logger->error("An SDL error occurred: {}", SDL_GetError());

      return 1;
    }

    SDL_GL_MakeCurrent(window.get(), context);

    window_colour_r = DEFAULT_WINDOW_CLEAR_COLOUR.r / 255.0f;
    window_colour_g = DEFAULT_WINDOW_CLEAR_COLOUR.g / 255.0f;
    window_colour_b = DEFAULT_WINDOW_CLEAR_COLOUR.b / 255.0f;

    window_render_mode = GL_FILL;

    logging::logger->debug("Created window.");

    return 0;
  }

  void CleanUpWindow()
  {
    SDL_GL_DeleteContext(context);
    window.reset();

    logging::logger->debug("Destroyed window.");
  }

  glm::ivec2 GetWindowSize()
  {
    glm::ivec2 size = glm::ivec2(0);
    SDL_GetWindowSize(window.get(), &size.x, &size.y);

    return size;
  }

  void SetWindowSize(const glm::ivec2& size)
  {
    SDL_SetWindowSize(window.get(), size.x, size.y);
    data::SetProjection(size);
  }

  void ResizeWindowToView()
  {
    SetWindowSize(fonts::glyph_size * views::view_size);
  }

  glm::vec3 GetWindowClearColour()
  {
    return glm::vec3(window_colour_r * 255, window_colour_g * 255, window_colour_b * 255);
  }

  void SetWindowClearColour(const glm::vec3& colour)
  {
    window_colour_r = colour.r / 255.0f;
    window_colour_g = colour.g / 255.0f;
    window_colour_b = colour.b / 255.0f;
  }

  void ClearWindow()
  {
    glPolygonMode(GL_FRONT_AND_BACK, window_render_mode);
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(window_colour_r, window_colour_g, window_colour_b, 1.0f);
  }

  void RefreshWindow()
  {
    SDL_GL_SwapWindow(window.get());
  }

  void EnableWireframe()
  {
    window_render_mode = GL_LINE;
  }

  void DisableWireframe()
  {
    window_render_mode = GL_FILL;
  }
}