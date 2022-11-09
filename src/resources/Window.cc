#include "Window.h"
#include "../resources/FontAtlas.h"
#include "../logging/Logger.h"
#include "../view/View.h"

namespace term_engine::system {
  SDL_GLContext context;
  SDL::Window window;
  glm::vec3 window_colour;
  GLuint window_render_mode;
  bool window_is_minimised;

  int InitWindow()
  {
    window = SDL::Window(SDL_CreateWindow("TermEngine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, DEFAULT_WIDTH, DEFAULT_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI));

    if (window == nullptr)
    {
      logging::logger->error("An SDL error occurred: {}", SDL_GetError());

      return 1;
    }

    context = SDL_GL_CreateContext(window.get());

    // GLEW needs to be initialised as soon as a GL context is created.
    if (context == NULL)
    {
      logging::logger->error("An SDL error occurred: {}", SDL_GetError());

      return 1;
    }

    SDL_GL_MakeCurrent(window.get(), context);
    SDL_GL_SetSwapInterval(0);

    window_colour = DEFAULT_WINDOW_CLEAR_COLOUR / 255.0f;
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

  glm::ivec2 GetWindowPosition()
  {
    glm::ivec2 pos = glm::ivec2();
    SDL_GetWindowPosition(window.get(), &pos.x, &pos.y);

    return pos;
  }

  void SetWindowPosition(const glm::ivec2& position)
  {
    SDL_SetWindowPosition(window.get(), position.x, position.y);
  }

  void CenterWindowPosition()
  {
    SDL_SetWindowPosition(window.get(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
  }

  glm::ivec2 GetWindowSize()
  {
    glm::ivec2 size = glm::ivec2();
    SDL_GetWindowSize(window.get(), &size.x, &size.y);

    return size;
  }

  void SetWindowSize(const glm::ivec2& size)
  {
    SDL_SetWindowSize(window.get(), size.x, size.y);
  }

  void ResizeWindowToView()
  {
    SetWindowSize(fonts::character_size * views::view_size);
  }

  bool IsWindowResizable()
  {
    return SDL_GetWindowFlags(window.get()) & SDL_WINDOW_RESIZABLE;
  }

  void SetWindowResizable(const bool& flag)
  {
    SDL_SetWindowResizable(window.get(), flag ? SDL_TRUE : SDL_FALSE);
  }

  bool IsWindowMinimised()
  {
    return SDL_GetWindowFlags(window.get()) & SDL_WINDOW_MINIMIZED;
  }

  bool IsWindowMaximised()
  {
    return SDL_GetWindowFlags(window.get()) & SDL_WINDOW_MAXIMIZED;
  }

  void MinimiseWindow()
  {
    SDL_MinimizeWindow(window.get());
  }

  void MaximiseWindow()
  {
    SDL_MaximizeWindow(window.get());
  }

  void RestoreWindow()
  {
    SDL_RestoreWindow(window.get());
  }

  void ShowWindow()
  {
    SDL_ShowWindow(window.get());
  }

  void HideWindow()
  {
    SDL_HideWindow(window.get());
  }

  bool IsMouseGrabbed()
  {
    return SDL_GetWindowMouseGrab(window.get());
  }

  void SetMouseGrab(const bool& flag)
  {
    SDL_SetWindowMouseGrab(window.get(), flag ? SDL_TRUE : SDL_FALSE);
  }

  glm::vec3 GetWindowClearColour()
  {
    return window_colour * 255.0f;
  }

  void SetWindowClearColour(const glm::vec3& colour)
  {
    window_colour = colour / 255.0f;
  }

  void ClearWindow()
  {
    glPolygonMode(GL_FRONT_AND_BACK, window_render_mode);
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(window_colour.r, window_colour.g, window_colour.b, 1.0f);
  }

  void RefreshWindow()
  {
    SDL_GL_SwapWindow(window.get());
  }

  bool IsVsyncEnabled()
  {
    return SDL_GL_GetSwapInterval() != 0;
  }

  void SetVsync(const bool& flag)
  {
    if (flag && SDL_GL_SetSwapInterval(-1) < 0)
    {
      SDL_GL_SetSwapInterval(1);
    }
    else
    {
      SDL_GL_SetSwapInterval(0);
    }
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