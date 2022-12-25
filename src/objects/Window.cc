#include "Window.h"
#include "../utility/SpdlogUtils.h"

namespace objects {
  Window::Window() :
    clear_colour_(DEFAULT_WINDOW_CLEAR_COLOUR / 255.0f),
    render_mode_(GL_FILL)
  {
    Initialise(glm::ivec2(SDL_WINDOWPOS_CENTERED), glm::ivec2(DEFAULT_WIDTH, DEFAULT_HEIGHT));

    if (is_init_)
    {
      SetTitle(std::string(DEFAULT_TITLE));
      SetVsync(vsync_flag_);
    }
  }

  Window::Window(const glm::ivec2& position, const glm::ivec2& size, const std::string& title) :
    clear_colour_(DEFAULT_WINDOW_CLEAR_COLOUR / 255.0f),
    render_mode_(GL_FILL)
  {
    Initialise(position, size);

    if (is_init_)
    {
      SetTitle(title);
      SetVsync(vsync_flag_);
    }
  }

  Window::~Window()
  {
    SDL_GL_DeleteContext(context_);
    window_.reset();

    logging::logger->debug("Destroyed window.");
  }

  glm::ivec2 Window::GetPosition() const
  {
    glm::ivec2 pos = glm::ivec2();
    SDL_GetWindowPosition(window_.get(), &pos.x, &pos.y);

    return pos;
  }

  glm::ivec2 Window::GetSize() const
  {
    glm::ivec2 size = glm::ivec2();
    SDL_GetWindowSize(window_.get(), &size.x, &size.y);

    return size;
  }

  std::string Window::GetTitle() const
  {
    return std::string(SDL_GetWindowTitle(window_.get()));
  }

  glm::vec4 Window::GetClearColour() const
  {
    return clear_colour_ * 255.0f;
  }

  bool Window::IsResizable() const
  {
    return SDL_GetWindowFlags(window_.get()) & SDL_WINDOW_RESIZABLE;
  }


  bool Window::IsMinimised() const
  {
    return SDL_GetWindowFlags(window_.get()) & SDL_WINDOW_MINIMIZED;
  }

  bool Window::IsMaximised() const
  {
    return SDL_GetWindowFlags(window_.get()) & SDL_WINDOW_MAXIMIZED;
  }


  bool Window::IsMouseGrabbed() const
  {
    return SDL_GetWindowMouseGrab(window_.get());
  }

  bool Window::IsWireframeEnabled() const
  {
    return render_mode_ == GL_LINE;
  }

  void Window::SetPosition(const glm::ivec2& position) const
  {
    SDL_SetWindowPosition(window_.get(), position.x, position.y);
  }

  void Window::CenterPosition() const
  {
    SDL_SetWindowPosition(window_.get(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
  }

  void Window::SetSize(const glm::ivec2& size) const
  {
    SDL_SetWindowSize(window_.get(), size.x, size.y);
  }

  void Window::SetResizable(const bool& flag) const
  {
    SDL_SetWindowResizable(window_.get(), flag ? SDL_TRUE : SDL_FALSE);
  }

  void Window::Minimise() const
  {
    SDL_MinimizeWindow(window_.get());
  }

  void Window::Maximise() const
  {
    SDL_MaximizeWindow(window_.get());
  }

  void Window::Restore() const
  {
    SDL_RestoreWindow(window_.get());
  }

  void Window::Show() const
  {
    SDL_ShowWindow(window_.get());
  }

  void Window::Hide() const
  {
    SDL_HideWindow(window_.get());
  }

  void Window::SetMouseGrab(const bool& flag) const
  {
    SDL_SetWindowMouseGrab(window_.get(), flag ? SDL_TRUE : SDL_FALSE);
  }

  void Window::SetClearColour(const glm::vec3& colour)
  {
    clear_colour_ = glm::vec4(colour, 255.0f) / 255.0f;
  }

  void Window::SetClearColour(const glm::vec4& colour)
  {
    clear_colour_ = colour / 255.0f;
  }

  bool IsVsyncEnabled()
  {
    return SDL_GL_GetSwapInterval() != 0;
  }

  void SetVsync(const GLint& flag)
  {
    if (flag >= -1 && flag <= 1)
    {
      if (SDL_GL_SetSwapInterval(flag) < 0)
      {
        logging::logger->error("Failed to set vsync flag: ", SDL_GetError());
      }
    }
    else
    {
      logging::logger->warn("Invalid vsync flag given.");
    }
  }

  void Window::SetWireframe(const bool& flag)
  {
    render_mode_ = flag ? GL_LINE : GL_FILL;
  }

  void Window::Initialise(const glm::ivec2& position, const glm::ivec2& size)
  {
    window_ = SDL::Window(SDL_CreateWindow("TermEngine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, DEFAULT_WIDTH, DEFAULT_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI));

    if (window_ == nullptr)
    {
      logging::logger->error("An SDL error occurred: {}", SDL_GetError());

      return;
    }

    context_ = SDL_GL_CreateContext(window_.get());

    // GLEW needs to be initialised as soon as a GL context is created.
    if (context_ == NULL)
    {
      logging::logger->error("An SDL error occurred: {}", SDL_GetError());

      return;
    }

    SDL_GL_MakeCurrent(window_.get(), context_);
    SDL_GL_SetSwapInterval(0);

    is_init_ = true;

    logging::logger->debug("Created window.");
  }

  void Window::Clear() const
  {
    SDL_GL_MakeCurrent(window_.get(), context_);
    glPolygonMode(GL_FRONT_AND_BACK, render_mode_);
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(clear_colour_.r, clear_colour_.g, clear_colour_.b, clear_colour_.a);
  }

  void Window::Refresh() const
  {
    SDL_GL_MakeCurrent(window_.get(), context_);
    SDL_GL_SwapWindow(window_.get());
  }
}