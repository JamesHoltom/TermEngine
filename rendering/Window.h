/*
 * Copyright 2020
 * James Holtom
 * MIT License
 */

#ifndef WINDOW_H
#define WINDOW_H

#include <memory>
#include <string>

#include "../sdl_includes.h"
#include "../gl_includes.h"

namespace term_engine {
  constexpr int DEFAULT_WINDOW_WIDTH = 640;
  constexpr int DEFAULT_WINDOW_HEIGHT = 480;

  class Window {
  public:

    Window(const std::string& name);
    ~Window();

    const std::string GetName() const;
    const int GetID() const;

    GLuint GetProgram() const;
    void SetProgram(const GLuint& program_id);
    void SetProgram(const std::string& program_name);
    void RemoveProgram();

    std::string GetTitle() const;
    void SetTitle(const std::string& title);

    glm::ivec2 GetPosition() const;
    void SetPosition(const int& x, const int& y);

    glm::ivec2 GetSize() const;
    void SetSize(const int& width, const int& height);

    void Logic(SDL_Event& event);
    void Render();

  private:

    static bool glew_init_;

    GLuint program_id_;

    SDL_GLContext context_;
    SDL_Window* window_;
    int window_id_;

    std::string name_;

    void Init();
  };
}

#endif
