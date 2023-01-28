#include "Window.h"
#include "../utility/SpdlogUtils.h"

namespace term_engine::rendering {
  Window::Window(const glm::ivec2& position, const glm::ivec2& size, const std::string& title, const Uint32& flags) :
    clear_colour_(DEFAULT_WINDOW_CLEAR_COLOUR / 255.0f),
    render_mode_(GL_FILL)
  {
    SetUpWindow(position, size, flags);

    if (is_init_)
    {
      SetTitle(title);
    }
  }

  Window::~Window()
  {
    window_.reset();

    utility::logger->debug("Destroyed window.");
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

    glViewport(0, 0, size.x, size.y);
  }

  void Window::SetClearColour(const glm::vec4& colour)
  {
    clear_colour_ = glm::clamp(colour, glm::vec4(), glm::vec4(255.0f));
  }

  void Window::SetTitle(const std::string& title) const
  {
    SDL_SetWindowTitle(window_.get(), title.c_str());
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

  void Window::SetWireframe(const bool& flag)
  {
    render_mode_ = flag ? GL_LINE : GL_FILL;
  }

  void Window::Refresh() const
  {
    SDL_GL_MakeCurrent(window_.get(), context_);
    SDL_GL_SwapWindow(window_.get());
  }

  bool Window::IsVsyncEnabled()
  {
    return SDL_GL_GetSwapInterval() != 0;
  }

  void Window::SetVsync(const GLint& flag)
  {
    if (flag >= -1 && flag <= 1)
    {
      if (SDL_GL_SetSwapInterval(flag) < 0)
      {
        utility::logger->error("Failed to set vsync flag: ", SDL_GetError());
      }
    }
    else
    {
      utility::logger->warn("Invalid vsync flag given.");
    }
  }

  void Window::CleanUpContext()
  {
    if (context_ != NULL)
    {
      SDL_GL_DeleteContext(context_);
    }
  }

  void Window::SetUpWindow(const glm::ivec2& position, const glm::ivec2& size, const Uint32& flags)
  {
    window_ = utility::SDLWindow(SDL_CreateWindow("TermEngine", position.x, position.y, size.x, size.y, SDL_WINDOW_OPENGL | flags));

    if (window_ == nullptr)
    {
      utility::logger->error("An SDL error occurred: {}", SDL_GetError());

      return;
    }

    if (!is_context_created_)
    {
      context_ = SDL_GL_CreateContext(window_.get());

      bool all_ok = context_ != NULL;

      if (all_ok)
      {
        // GLEW needs to be initialised as soon as a GL context is created.
        all_ok = utility::InitGLEW();
      }
      else
      {
        utility::logger->error("An error occurred whilst creating the context: {}", SDL_GetError());

        return;
      }

      if (all_ok)
      {
        is_context_created_ = true;
      }
      else
      {
        return;
      }
    }

    SDL_GL_MakeCurrent(window_.get(), context_);
    SDL_GL_SetSwapInterval(vsync_flag_);

    glViewport(0, 0, size.x, size.y);

    is_init_ = true;

    utility::logger->debug("Created window.");
  }

  void Window::Clear() const
  {
    SDL_GL_MakeCurrent(window_.get(), context_);
    glPolygonMode(GL_FRONT_AND_BACK, render_mode_);
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(clear_colour_.r, clear_colour_.g, clear_colour_.b, clear_colour_.a);
  }

  bool Window::is_context_created_ = false;
  SDL_GLContext Window::context_;
  GLint Window::vsync_flag_ = 0;
}