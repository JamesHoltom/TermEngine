#include <spdlog/spdlog.h>
#include <tuple>

#include "../init.h"
#include "Window.h"

namespace term_engine::windows {
  Window::Window(const std::string& name) :
    shader_program_(nullptr),
    context_(nullptr),
    window_(nullptr),
    window_id_(0),
    name_(name)
  {
    Init();
  }

  Window::~Window() {
    SDL_GL_DeleteContext(context_);
    context_ = nullptr;

    SDL_DestroyWindow(window_);
    window_ = nullptr;

    spdlog::info("Destroyed window {}.", name_);
  }

  void Window::Init() {

    window_ = SDL_CreateWindow("TermEngine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, SDL_WINDOW_OPENGL);

    if (window_ != NULL) {
      window_id_ = SDL_GetWindowID(window_);
    }
    else {
      spdlog::error("An SDL error occurred: {}", SDL_GetError());

      return;
    }

    context_ = SDL_GL_CreateContext(window_);

    // GLEW needs to be initialised as soon as a GL context is created.
    if (context_ != NULL) {
      if (glew_init_ == false) {
        InitGLEW();

        glew_init_ = true;
      }
    }
    else {
      spdlog::error("An SDL error occurred: {}", SDL_GetError());

      return;
    }
  }

  const std::string Window::GetName() const {
    return name_;
  }

  const int Window::GetID() const {
    return window_id_;
  }

  void Window::SetShaderProgram(const std::string& program_name) {
    shader_program_ = shaders::GetShader(program_name);

    spdlog::info("Window {} has program name {}, and ID {}.", name_, program_name, shader_program_->GetProgramID());
  }

  void Window::RemoveShaderProgram() {
    shader_program_ = nullptr;

    spdlog::info("Removed program from window {}.", name_);
  }

  void Window::SetGlyphSet(const std::string& glyph_set_name) {
    glyph_set_ = glyphs::GetGlyphSet(glyph_set_name);

    spdlog::info("Window {} has glyph set name {}.", name_, glyph_set_name);
  }

  void Window::RemoveGlyphSet() {
    glyph_set_ = nullptr;

    spdlog::info("Removed glyph set from window {}.", name_);
  }

  std::string Window::GetTitle() const {
    return SDL_GetWindowTitle(window_);
  }

  void Window::SetTitle(const std::string& title) {
    SDL_SetWindowTitle(window_, title.c_str());
  }

  glm::ivec2 Window::GetPosition() const {
    glm::ivec2 position;

    SDL_GetWindowPosition(window_, &position.x, &position.y);

    return position;
  }

  void Window::SetPosition(const int& x, const int& y) {
    SDL_SetWindowPosition(window_, x, y);
  }

  glm::ivec2 Window::GetSize() const {
    glm::ivec2 size;

    SDL_GetWindowSize(window_, &size.x, &size.y);

    return size;
  }

  void Window::SetSize(const int& width, const int& height) {
    SDL_SetWindowSize(window_, width, height);
  }

  void Window::Logic(SDL_Event& event) {
    if (event.type == SDL_KEYUP) {
      switch (event.key.keysym.sym) {
      case SDLK_p:
        glyph_set_->Print();

        break;
      }
    }
  }

  void Window::Render() const {
    SDL_GL_MakeCurrent(window_, context_);

    if (shader_program_->IsBuilt()) {
      glClearColor(0.2f, 0.2f, 0.4f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      shader_program_->Use();
      glyph_set_->Render();
      shader_program_->Unuse();
    }
    else {
      glClearColor(0.4f, 0.1f, 0.1f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);
    }

    SDL_GL_SwapWindow(window_);
  }

  bool Window::glew_init_ = false;
}