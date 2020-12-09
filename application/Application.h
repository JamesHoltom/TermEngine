/*
 * Copyright 2020
 * James Holtom
 * MIT License
 */

#ifndef APPLICATION_H
#define APPLICATION_H

#include "../fonts/FontAtlas.h"
#include "../glyphs/GlyphSet.h"
#include "../logging/Logger.h"
#include "../scenes/Scene.h"
#include "../scripting/ScriptingInterface.h"
#include "../shaders/Shader.h"
#include "../utility/SDLUtils.h"

namespace term_engine::application {
  void Init();
  void CleanUp();
  void Run();

  void ChangeFont(const int& pattern);
  void ChangeSet(const int& pattern);
}

#endif // ! APPLICATION_H
