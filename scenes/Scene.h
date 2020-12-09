/*
 * Copyright 2020
 * James Holtom
 * MIT License
 */

#ifndef SCENE_H
#define SCENE_H

#include <memory>
#include <string>
#include <unordered_map>

#include "../glyphs/GlyphSet.h"

namespace term_engine::scenes {
  class Scene {
  public:
    Scene();

    void Render() const;

    glyphs::GlyphSetPtr GetGlyphSet();

  private:
    glyphs::GlyphSetPtr glyph_set_;
  };

  typedef std::shared_ptr<Scene> ScenePtr;
  typedef std::unordered_map<std::string, ScenePtr> SceneList;
}

#endif // ! SCENE_H
