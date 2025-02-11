#include "Window.h"
#include "../utility/GLUtils.h"
#include "../utility/ImGuiUtils.h"
#include "../utility/LogUtils.h"

namespace term_engine::usertypes {
  Window::Window(GameWindow* game_window, const glm::ivec2& position, const glm::ivec2& size, const std::string& title, uint32_t flags) :
    game_window_(game_window),
    window_(nullptr),
    size_(size),
    clear_colour_(DEFAULT_WINDOW_CLEAR_COLOUR / 255.0f),
    render_mode_(GL_FILL)
  {
    window_ = SDL_CreateWindow(title.c_str(), position.x, position.y, size.x, size.y, SDL_WINDOW_OPENGL | flags);

    if (window_ == nullptr)
    {
      utility::LogError("An SDL error occurred: {}", SDL_GetError());

      return;
    }

    SDL_GetWindowPosition(window_, &position_.x, &position_.y);

    SDL_GL_MakeCurrent(window_, utility::context);
    SDL_GL_SetSwapInterval(vsync_flag_);

    is_init_ = true;

    utility::LogDebug("Created window with window ID {}.", GetId());
  }

  Window::~Window()
  {
    SDL_DestroyWindow(window_);

    utility::LogDebug("Destroyed window.");
  }

  void Window::DoEvents(const SDL_Event& event)
  {
    if (event.window.event == SDL_WINDOWEVENT_MOVED)
    {
      SetPosition(glm::ivec2(event.window.data1, event.window.data2));
    }
    else if (event.window.event == SDL_WINDOWEVENT_RESIZED)
    {
      SetSize(glm::ivec2(event.window.data1, event.window.data2));
    }
  }

  uint32_t Window::GetId() const
  {
    return SDL_GetWindowID(window_);
  }

  SDL_Window* Window::GetWindow()
  {
    return window_;
  }

  glm::ivec2& Window::GetPosition()
  {
    return position_;
  }

  glm::ivec2 &Window::GetSize()
  {
    return size_;
  }

  std::string Window::GetTitle() const
  {
    return std::string(SDL_GetWindowTitle(window_));
  }

  glm::vec4& Window::GetClearColour()
  {
    return clear_colour_;
  }

  bool Window::IsBorderless() const
  {
    return SDL_GetWindowFlags(window_) & SDL_WINDOW_BORDERLESS;
  }

  bool Window::IsResizable() const
  {
    return SDL_GetWindowFlags(window_) & SDL_WINDOW_RESIZABLE;
  }

  bool Window::IsMinimised() const
  {
    return SDL_GetWindowFlags(window_) & SDL_WINDOW_MINIMIZED;
  }

  bool Window::IsMaximised() const
  {
    return SDL_GetWindowFlags(window_) & SDL_WINDOW_MAXIMIZED;
  }

  bool Window::IsInFocus() const
  {
    return window_ == SDL_GetMouseFocus();
  }

  bool Window::IsMouseGrabbed() const
  {
    return SDL_GetWindowMouseGrab(window_);
  }

  bool Window::IsWireframeEnabled() const
  {
    return render_mode_ == GL_LINE;
  }

  void Window::Reposition(const glm::ivec2& position)
  {
    SDL_SetWindowPosition(window_, position.x, position.y);
    
    SetPosition(position);
  }

  void Window::CenterPosition()
  {
    Reposition(glm::ivec2(SDL_WINDOWPOS_CENTERED));
  }

  void Window::Resize(const glm::ivec2& size)
  {
    if (size.x <= 0 || size.y <= 0)
    {
      utility::LogWarn("Cannot set window size to 0 or less!");

      return;
    }

    SDL_SetWindowSize(window_, size.x, size.y);

    SetSize(size);
  }

  void Window::SetPosition(const glm::ivec2& position)
  {
    position_ = position;
  }

  void Window::SetSize(const glm::ivec2& size)
  {
    assert(size.x > 0 && size.y > 0);

    size_ = size;
    game_window_->SetProjection(size);
  }

  void Window::SetTitle(const std::string& title) const
  {
    SDL_SetWindowTitle(window_, title.c_str());
  }

  void Window::SetClearColour(const glm::vec4& colour)
  {
    clear_colour_ = glm::clamp(colour, glm::vec4(), glm::vec4(255.0f));
  }

  void Window::SetBorderless(bool flag) const
  {
    SDL_SetWindowBordered(window_, flag ? SDL_TRUE : SDL_FALSE);
  }

  void Window::SetResizable(bool flag) const
  {
    SDL_SetWindowResizable(window_, flag ? SDL_TRUE : SDL_FALSE);
  }

  void Window::Minimise() const
  {
    SDL_MinimizeWindow(window_);
  }

  void Window::Maximise() const
  {
    SDL_MaximizeWindow(window_);
  }

  void Window::Restore() const
  {
    SDL_RestoreWindow(window_);
  }

  void Window::Show() const
  {
    SDL_ShowWindow(window_);
  }

  void Window::Hide() const
  {
    SDL_HideWindow(window_);
  }

  void Window::SetMouseGrab(bool flag) const
  {
    SDL_SetWindowMouseGrab(window_, flag ? SDL_TRUE : SDL_FALSE);
  }

  void Window::SetWireframe(bool flag)
  {
    render_mode_ = flag ? GL_LINE : GL_FILL;
  }

  void Window::Use() const
  {
    SDL_GL_MakeCurrent(window_, utility::context);
  }

  void Window::Clear() const
  {
    Use();
    
    glPolygonMode(GL_FRONT_AND_BACK, render_mode_);
    glClearColor(clear_colour_.r / 255.0f, clear_colour_.g / 255.0f, clear_colour_.b / 255.0f, clear_colour_.a / 255.0f);
    glClear(GL_COLOR_BUFFER_BIT);
  }

  void Window::Refresh() const
  {
    SDL_GL_SwapWindow(window_);
  }

  void Window::SetGameWindow(GameWindow* ptr)
  {
    game_window_ = ptr;
  }

  void Window::UpdateDebugInfo() const
  {
    ImGui::SeparatorText("Window");

    ImGui::Text("ID: %u", SDL_GetWindowID(window_));
    ImGui::Text("Title: %s", SDL_GetWindowTitle(window_));
    ImGui::Text("Position: %i, %i", position_.x, position_.y);
    ImGui::Text("Size: %i, %i", size_.x, size_.y);
    ImGui::Text("Clear Colour: %f, %f, %f, %f", clear_colour_.r, clear_colour_.g, clear_colour_.b, clear_colour_.a);
    ImGui::Text("Focussed: %s", IsInFocus() ? "Yes" : "No");
  }

  bool Window::IsVsyncEnabled()
  {
    return SDL_GL_GetSwapInterval() != 0;
  }

  void Window::SetVsync(int flag)
  {
    if (flag >= -1 && flag <= 1)
    {
      if (SDL_GL_SetSwapInterval(flag) < 0)
      {
        utility::LogError("Failed to set vsync flag: ", SDL_GetError());
      }
    }
    else
    {
      utility::LogWarn("Invalid vsync flag given.");
    }
  }

  bool InitDefaultWindow()
  {
    default_window = std::make_unique<Window>(nullptr, DEFAULT_WINDOW_POSITION, DEFAULT_WINDOW_SIZE, std::string(DEFAULT_WINDOW_TITLE), 0);

    // GLEW needs to be initialised as soon as a GL context is created.
    if (!utility::InitContext(default_window->GetWindow()))
    {
      utility::LogError("Failed to set up OpenGL context!");
      
      return false;
    }

    return true;
  }

  void CleanUpDefaultWindow()
  {
    default_window.release();

    utility::CleanUpContext();
  }

  int Window::vsync_flag_ = 0;
}