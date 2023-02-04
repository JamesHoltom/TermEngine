#include "Window.h"
#include "../utility/SpdlogUtils.h"

namespace term_engine::rendering {
  GameWindow::GameWindow() :
    window_(nullptr),
    clear_colour_(DEFAULT_WINDOW_CLEAR_COLOUR / 255.0f),
    render_mode_(GL_FILL)
  {
    SetUpWindow(DEFAULT_POSITION, DEFAULT_SIZE, 0);

    if (is_init_)
    {
      SetTitle(std::string(DEFAULT_TITLE));
    }
  }

  GameWindow::GameWindow(const glm::ivec2& position, const glm::ivec2& size, const std::string& title, const Uint32& flags) :
    window_(nullptr),
    clear_colour_(DEFAULT_WINDOW_CLEAR_COLOUR / 255.0f),
    render_mode_(GL_FILL)
  {
    SetUpWindow(position, size, flags);

    if (is_init_)
    {
      SetTitle(title);
    }
  }

  GameWindow::~GameWindow()
  {
    SDL_DestroyWindow(window_);
    CleanUpContext();

    utility::logger->debug("Destroyed window.");
  }

  Uint32 GameWindow::GetId() const
  {
    return SDL_GetWindowID(window_);
  }

  glm::ivec2 GameWindow::GetPosition() const
  {
    glm::ivec2 pos = glm::ivec2();
    SDL_GetWindowPosition(window_, &pos.x, &pos.y);

    return pos;
  }

  glm::ivec2 GameWindow::GetSize() const
  {
    glm::ivec2 size = glm::ivec2();
    SDL_GetWindowSize(window_, &size.x, &size.y);

    return size;
  }

  std::string GameWindow::GetTitle() const
  {
    return std::string(SDL_GetWindowTitle(window_));
  }

  glm::vec4 GameWindow::GetClearColour() const
  {
    return clear_colour_ * 255.0f;
  }

  bool GameWindow::IsResizable() const
  {
    return SDL_GetWindowFlags(window_) & SDL_WINDOW_RESIZABLE;
  }


  bool GameWindow::IsMinimised() const
  {
    return SDL_GetWindowFlags(window_) & SDL_WINDOW_MINIMIZED;
  }

  bool GameWindow::IsMaximised() const
  {
    return SDL_GetWindowFlags(window_) & SDL_WINDOW_MAXIMIZED;
  }


  bool GameWindow::IsMouseGrabbed() const
  {
    return SDL_GetWindowMouseGrab(window_);
  }

  bool GameWindow::IsWireframeEnabled() const
  {
    return render_mode_ == GL_LINE;
  }

  void GameWindow::SetPosition(const glm::ivec2& position) const
  {
    SDL_SetWindowPosition(window_, position.x, position.y);
  }

  void GameWindow::CenterPosition() const
  {
    SDL_SetWindowPosition(window_, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
  }

  void GameWindow::SetSize(const glm::ivec2& size) const
  {
    SDL_SetWindowSize(window_, size.x, size.y);

    glViewport(0, 0, size.x, size.y);
  }

  void GameWindow::SetClearColour(const glm::vec4& colour)
  {
    clear_colour_ = glm::clamp(colour, glm::vec4(), glm::vec4(255.0f));
  }

  void GameWindow::SetTitle(const std::string& title) const
  {
    SDL_SetWindowTitle(window_, title.c_str());
  }

  void GameWindow::SetResizable(const bool& flag) const
  {
    SDL_SetWindowResizable(window_, flag ? SDL_TRUE : SDL_FALSE);
  }

  void GameWindow::Minimise() const
  {
    SDL_MinimizeWindow(window_);
  }

  void GameWindow::Maximise() const
  {
    SDL_MaximizeWindow(window_);
  }

  void GameWindow::Restore() const
  {
    SDL_RestoreWindow(window_);
  }

  void GameWindow::Show() const
  {
    SDL_ShowWindow(window_);
  }

  void GameWindow::Hide() const
  {
    SDL_HideWindow(window_);
  }

  void GameWindow::SetMouseGrab(const bool& flag) const
  {
    SDL_SetWindowMouseGrab(window_, flag ? SDL_TRUE : SDL_FALSE);
  }

  void GameWindow::SetWireframe(const bool& flag)
  {
    render_mode_ = flag ? GL_LINE : GL_FILL;
  }

  void GameWindow::Use() const
  {
    SDL_GL_MakeCurrent(window_, context_);
  }

  void GameWindow::Clear() const
  {
    SDL_GL_MakeCurrent(window_, context_);
    glPolygonMode(GL_FRONT_AND_BACK, render_mode_);
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(clear_colour_.r, clear_colour_.g, clear_colour_.b, clear_colour_.a);
  }

  void GameWindow::Refresh() const
  {
    SDL_GL_SwapWindow(window_);
  }

  bool GameWindow::IsVsyncEnabled()
  {
    return SDL_GL_GetSwapInterval() != 0;
  }

  void GameWindow::SetVsync(const GLint& flag)
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

  void GameWindow::CleanUpContext()
  {
    if (context_ != NULL)
    {
      SDL_GL_DeleteContext(context_);
    }
  }

  void GameWindow::SetUpWindow(const glm::ivec2& position, const glm::ivec2& size, const Uint32& flags)
  {
    window_ = SDL_CreateWindow("TermEngine", position.x, position.y, size.x, size.y, SDL_WINDOW_OPENGL | flags);

    if (window_ == nullptr)
    {
      utility::logger->error("An SDL error occurred: {}", SDL_GetError());

      return;
    }

    if (!is_context_created_)
    {
      context_ = SDL_GL_CreateContext(window_);

      if (context_ != NULL)
      {
        // GLEW needs to be initialised as soon as a GL context is created.
        if (utility::InitGLEW())
        {
          is_context_created_ = true;
        }
        else
        {
          utility::logger->error("Failed to set up OpenGL context!");
          
          return;
        }
      }
      else
      {
        utility::logger->error("An error occurred whilst creating the context: {}", SDL_GetError());

        return;
      }
    }

    SDL_GL_MakeCurrent(window_, context_);
    SDL_GL_SetSwapInterval(vsync_flag_);

    glViewport(0, 0, size.x, size.y);

    is_init_ = true;

    utility::logger->debug("Created window.");
  }

  bool GameWindow::is_context_created_ = false;
  SDL_GLContext GameWindow::context_;
  GLint GameWindow::vsync_flag_ = 0;
}