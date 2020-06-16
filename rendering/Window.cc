#include <spdlog/spdlog.h>
#include <tuple>

#include "../init.h"
#include "../rendering/ShaderManager.h"
#include "VertexManager.h"
#include "Window.h"

namespace term_engine {
  Window::Window(const std::string& name) :
    program_id_(0),
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

  GLuint Window::GetProgram() const {
    return program_id_;
  }

  void Window::SetProgram(const GLuint& program_id) {
    program_id_ = program_id;

    spdlog::info("Window {} has program ID {}.", name_, program_id_);
  }

  void Window::SetProgram(const std::string& program_name) {
    program_id_ = shaders::GetProgram(program_name);

    spdlog::info("Window {} has program name {}, and ID {}.", name_, program_name, program_id_);
  }

  void Window::RemoveProgram() {
    program_id_ = 0;

    spdlog::info("Removed program from window {}.", name_);
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

  }

  void Window::Render() {
    SDL_GL_MakeCurrent(window_, context_);

    if (program_id_ > 0) {
      glClearColor(0.2f, 0.2f, 0.4f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);

      glUseProgram(program_id_);

      glBindVertexArray(vertex_array_object_id_);

      glDrawArrays(GL_QUADS, 0, 4);

      glUseProgram(NULL);
    }
    else {
      glClearColor(0.4f, 0.1f, 0.1f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);
    }

    SDL_GL_SwapWindow(window_);
  }

  bool Window::glew_init_ = false;
}