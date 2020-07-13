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
#include "../shaders/Shader.h"
#include "../shaders/ShaderManager.h"
#include "../text/GlyphSet.h"
#include "../text/GlyphManager.h"

namespace term_engine::windows {
  constexpr int DEFAULT_WINDOW_WIDTH = 640;
  constexpr int DEFAULT_WINDOW_HEIGHT = 480;

  class Window {
  public:

    Window(const std::string& name);
    ~Window();

    const std::string GetName() const;
    const int GetID() const;

    void SetShaderProgram(const std::string& program_name);
    void RemoveShaderProgram();

    void SetGlyphSet(const std::string& glyph_set_name);
    void RemoveGlyphSet();

    std::string GetTitle() const;
    void SetTitle(const std::string& title);

    glm::ivec2 GetPosition() const;
    void SetPosition(const int& x, const int& y);

    glm::ivec2 GetSize() const;
    void SetSize(const int& width, const int& height);

    void Logic(SDL_Event& event);
    void Render() const;

  private:

    static bool glew_init_;

    shaders::ShaderPtr shader_program_;
    glyphs::GlyphSetPtr glyph_set_;

    SDL_GLContext context_;
    SDL_Window* window_;
    int window_id_;

    std::string name_;

    void Init();
  };
}

#endif
