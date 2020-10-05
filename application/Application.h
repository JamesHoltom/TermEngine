/*
 * Copyright 2020
 * James Holtom
 * MIT License
 */

#ifndef APPLICATION_H
#define APPLICATION_H

#include "../fonts/FontAtlasManager.h"
#include "../rendering/GlyphManager.h"
#include "../shaders/ShaderManager.h"

#include "../utility/SDLUtils.h"

namespace term_engine {
  class Application {
  public:
    Application();
    ~Application();

    void Run();

  private:
    SDL_Event event_;
    bool quit_;

    shaders::ShaderPtr shader_;
  };
}

#endif // !APPLICATION_H