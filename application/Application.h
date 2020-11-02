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
#include "../scripting/ScriptManager.h"

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
    scripting::ScriptManager script_;
  };

  void ChangeSet(glyphs::GlyphSetPtr& set, fonts::FontAtlasPtr& atlas, const int& pattern);
}

#endif // !APPLICATION_H
