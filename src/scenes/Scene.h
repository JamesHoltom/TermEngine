/// @author James Holtom

#ifndef SCENE_H
#define SCENE_H

#include <memory>
#include <string>
#include <unordered_map>

#include "../glyphs/GlyphSet.h"

namespace term_engine::scenes {
  class Scene;

  /// Smart pointer used to share a single instance of a scene.
  typedef std::shared_ptr<Scene> ScenePtr;
  /// Used to store a list of scenes.
  typedef std::unordered_map<std::string, ScenePtr> SceneList;

  /// Contains the glyph set and other game objects that make up a menu, level, etc.
  class Scene {
  public:
    /// Constructs the scene.
    Scene();

    /// Destroys the scene.
    ~Scene();

    /// Renders the contents of the scene to the window.
    void Render() const;

    /// Returns the glyph set tied to the scene.
    /**
     * @returns A pointer to the scene's glyph set.
     */
    glyphs::GlyphSetPtr GetGlyphSet();

  private:
    /// A pointer to the glyph set rendered in this scene.
    glyphs::GlyphSetPtr glyph_set_;
  };
}

#endif // ! SCENE_H
