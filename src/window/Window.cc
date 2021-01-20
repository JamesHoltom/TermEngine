#include "Window.h"
#include "../utility/init.h"
#include "../shaders/CommonUniform.h"
#include "../logging/Logger.h"

namespace term_engine::windows {
  SDL_GLContext context;
  SDL::Window window;
  GLfloat window_color_r, window_color_g, window_color_b;
  GLuint window_render_mode;

  void Init()
  {
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

    window_color_r = DEFAULT_WINDOW_CLEAR_COLOR.r / 255.0f;
    window_color_g = DEFAULT_WINDOW_CLEAR_COLOR.g / 255.0f;
    window_color_b = DEFAULT_WINDOW_CLEAR_COLOR.b / 255.0f;

    window_render_mode = GL_FILL;

    logging::logger->debug("Created window.");
  }

  void CleanUp()
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
    shaders::ResetProjection();
  }

  glm::uvec3 GetWindowClearColor()
  {
    return glm::uvec3(window_color_r * 255, window_color_g * 255, window_color_b * 255);
  }

  void SetWindowClearColor(const glm::uvec3& color)
  {
    window_color_r = color.r / 255.0f;
    window_color_g = color.g / 255.0f;
    window_color_b = color.b / 255.0f;
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