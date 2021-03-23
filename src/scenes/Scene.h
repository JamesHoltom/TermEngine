/// @author James Holtom

#ifndef SCENE_H
#define SCENE_H

#include <memory>
#include <string>
#include <unordered_map>

#include "../fonts/FontAtlas.h"
#include "../glyphs/GlyphSet.h"
#include "../shaders/Shader.h"

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
    glyphs::GlyphSetPtr GetGlyphSet() const;

    /// Returns the font atlas used by the scene.
    /**
     * @returns A pointer to the scene's font atlas.
     */
    fonts::FontAtlasPtr GetFont() const;

    /// Returns the shader used for rendering.
    /**
     * @returns A pointer to the scene's shader.
     */
    shaders::ShaderPtr GetShader() const;

    /// Sets the font atlas used by the scene.
    /**
     * If the font atlas does not exist, nothing is changed.
     * @param[in] key The font path/size pair to set the font atlas to.
     */
    void SetFont(const fonts::FontAtlasKey& key);

    /// Sets the font atlas used by the scene.
    /**
     * @param[in] font A pointer to the font atlas to use.
     */
    void SetFont(const fonts::FontAtlasPtr& font);

    /// Sets the shader used by the scene.
    /**
     * If the shader does not exist, nothing is changed.
     * @param[in] shader_name The name of the shader to set in the scene.
     */
    void SetShader(const std::string& shader_name);

    /// Sets the shader used by the scene.
    /**
     * @param[in] shader A pointer to the shader to use.
     */
    void SetShader(const shaders::ShaderPtr& shader);

  private:
    /// A pointer to the glyph set being displayed.
    glyphs::GlyphSetPtr glyph_set_;

    /// A pointer to the font atlas used to render glyphs.
    fonts::FontAtlasPtr font_atlas_;

    /// A pointer to the shader rendering to the scene.
    shaders::ShaderPtr shader_;
  };
}

#endif // ! SCENE_H
