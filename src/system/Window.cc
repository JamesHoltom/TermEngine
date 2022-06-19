#include "Window.h"
#include "../data/Uniform.h"
#include "../resources/FontAtlas.h"
#include "../logging/Logger.h"
#include "../view/View.h"

namespace term_engine::system {
  SDL_GLContext context;
  SDL::Window window;
  GLfloat window_color_r, window_color_g, window_color_b;
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

    window_color_r = DEFAULT_WINDOW_CLEAR_COLOR.r / 255.0f;
    window_color_g = DEFAULT_WINDOW_CLEAR_COLOR.g / 255.0f;
    window_color_b = DEFAULT_WINDOW_CLEAR_COLOR.b / 255.0f;

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

  glm::vec3 GetWindowClearColor()
  {
    return glm::vec3(window_color_r * 255, window_color_g * 255, window_color_b * 255);
  }

  void SetWindowClearColor(const glm::vec3& color)
  {
    window_color_r = color.r / 255.0f;
    window_color_g = color.g / 255.0f;
    window_color_b = color.b / 255.0f;
  }

  void ClearWindow()
  {
    glPolygonMode(GL_FRONT_AND_BACK, window_render_mode);
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(window_color_r, window_color_g, window_color_b, 1.0f);
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