/// @author James Holtom

#ifndef GAME_SCENE_H
#define GAME_SCENE_H

#include <memory>
#include <string>
#include <vector>
#include "BaseObject.h"
#include "../rendering/Background.h"
#include "../rendering/FontAtlas.h"
#include "../rendering/ShaderProgram.h"
#include "../rendering/Buffer.h"
#include "../rendering/CharacterMap.h"
#include "../rendering/Window.h"

namespace term_engine::objects {
  class GameScene;

  /// @brief The name of the default game scene.
  constexpr char DEFAULT_GAME_SCENE_NAME[] = "default";
  /// @brief The type name for GameScenes.
  constexpr char GAME_SCENE_TYPE[] = "GameScene";

  /// @brief Manages a window on the user's screen, which other game objects and event listeners can be added to.
  class GameScene : public BaseObject {
  public:
    /**
     * @brief Constructs the game scene with the given name.
     * 
     * @param[in] name The name of the scene.
     */
    GameScene(const std::string& name);

    /// @brief Destroys the game scene.
    ~GameScene();

    /// @brief Updates the game scene.
    void Update();

    /**
     * @brief Returns the type of the object.
     * 
     * @returns The object type.
     */
    std::string GetObjectType() const;

    /**
     * @brief Returns the list priority for this object.
     * @details This is used to sort the list of objects before updating.
     * 
     * @returns The priority for this object.
     */
    ObjectSortPriority GetListPriority() const;

    /**
     * @brief Returns the name of the game scene.
     * 
     * @returns The scene name.
     */
    std::string GetName() const;

    /**
     * @brief Returns the font atlas used by the game scene.
     * 
     * @returns The raw pointer to the font atlas.
     */
    rendering::FontAtlas* GetFontAtlas();

    /**
     * @brief Returns the font size this scene renders characters at.
     * 
     * @returns The scene's font size.
     */
    uint32_t GetFontSize() const;

    /**
     * @brief Returns the background rendered behind the game scene.
     * 
     * @returns The raw pointer to the background.
     */
    rendering::Background* GetBackground();

    /**
     * @brief Returns the shader program used to render backgrounds to the game scene.
     * 
     * @returns The raw pointer to the shader program.
     */
    rendering::ShaderProgram* GetBackgroundShader();

    /**
     * @brief Returns the shader program used to render characters to the game scene.
     * 
     * @returns The raw pointer to the shader program.
     */
    rendering::ShaderProgram* GetTextShader();

    /**
     * @brief Returns the character map so that objects can be drawn to it.
     * 
     * @returns The raw pointer to the character map.
     */
    rendering::CharacterMap* GetCharacterMap();

    /**
     * @brief Returns the window handle used to render the game scene.
     * 
     * @returns The raw pointer to the window.
     */
    rendering::GameWindow* GetWindow();

    /**
     * @brief Sets the font atlas used by the game scene.
     * 
     * @param[in] font_atlas A raw pointer to the font atlas.
     */
    void SetFontAtlas(rendering::FontAtlas* font_atlas);

    /**
     * @brief Sets the font size this scene renders characters at.
     * 
     * @param[in] font_size The new font size.
     */
    void SetFontSize(uint32_t font_size);

    /// @brief Resizes the window to fit the character map.
    void ResizeToFitCharacterMap() const;

    /// @brief A list of raw pointers to the game scenes.
    static std::vector<GameScene*> scene_list_;
    /// @brief Flag to indicate if the user wants to quit the program.
    static bool quit_flag_;

  protected:
    /// @brief The name of the game scene.
    std::string name_;
    /// @brief A handle to the window the game scene renders to.
    rendering::GameWindow window_;
    /// @brief The background texture rendered behind the game scene.
    rendering::Background background_;
    /// @brief The list of characters to render to the game scene.
    rendering::CharacterMap character_map_;
    /// @brief The buffer containing vertex data for characters.
    rendering::Buffer text_buffer_;
    /// @brief The buffer containing vertex data for backgrounds.
    rendering::Buffer background_buffer_;
    /// @brief The font atlas used to render characters to the game scene.
    rendering::FontAtlas* font_atlas_;
    /// @brief The font size to render characters at, in pixels (px).
    uint32_t font_size_;
    /// @brief The shader program used to render background elements to the game scene.
    rendering::ShaderProgram background_shader_program_;
    /// @brief The shader program used to render text elements to the game scene.
    rendering::ShaderProgram text_shader_program_;

    /// @brief Re-sets the projection matrix in the scene's shader, according to the size of the window.
    void ResetProjection() const;
  };

  /**
   * @brief Adds a game scene to the list.
   * 
   * @param[in] name The name of the game scene.
   * @returns A raw pointer to the object.
   */
  GameScene* AddGameScene(const std::string& name);

  /**
   * @brief Returns the game scene with the given name.
   * 
   * @param[in] name The name of the game scene.
   * @returns A raw pointer to the object, or a null pointer if not found.
   */
  GameScene* GetGameSceneByName(const std::string& name);

  /**
   * @brief Flags the game scene with the given window ID for removal.
   * @details Game scenes are not automatically removed when the user closes a window:
   *          unless a script calls the scene's "preventClose()", the game scene will
   *          close after all objects have been updated.
   * 
   * @param[in] windowId The ID of the window related to the game scene.
   */
  void FlagGameSceneForRemoval(uint32_t windowId);

  /// @brief Flags game objects that are related to game scenes that have been flagged for removal.
  void FlagGameObjectsWithFlaggedGameScenes();

  /// @brief Clears all game scenes from the object list.
  void ClearAllGameScenes();
}

#endif // ! GAME_SCENE_H