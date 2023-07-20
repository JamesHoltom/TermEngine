/// @author James Holtom

#ifndef GAME_SCENE_H
#define GAME_SCENE_H

#include <memory>
#include <string>
#include <unordered_map>
#include "Background.h"
#include "CharacterMap.h"
#include "Window.h"
#include "resources/Font.h"
#include "resources/ShaderProgram.h"
#include "../rendering/Buffer.h"
#include "../utility/LogUtils.h"

namespace term_engine::usertypes {
  class GameScene;

  /// @brief Unique pointer to a game scene.
  typedef std::unique_ptr<GameScene> GameScenePtr;
  /// @brief Used to store a list of game scenes.
  typedef std::unordered_map<std::string, GameScenePtr> GameSceneList;

  /// @brief The name of the default game scene.
  constexpr char DEFAULT_GAME_SCENE_NAME[] = "default";

  /// @brief A list of raw pointers to the game scenes.
  inline GameSceneList game_scene_list;
  /// @brief Flag to indicate if the user wants to quit the program.
  inline bool quit_flag = false;

  /// @brief Manages a window on the user's screen, which other game objects can be added to.
  class GameScene {
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
     * @brief Returns the name of the game scene.
     * 
     * @returns The scene name.
     */
    std::string GetName() const;

    /**
     * @brief Returns the font size this scene renders characters at.
     * 
     * @returns The scene's font size.
     */
    uint32_t GetFontSize() const;

    /**
     * @brief Returns the background rendered behind the game scene.
     * 
     * @returns A raw pointer to the background.
     */
    Background* GetBackground();

    /**
     * @brief Returns the character map so that objects can be drawn to it.
     * 
     * @returns A raw pointer to the character map.
     */
    CharacterMap* GetCharacterMap();

    /**
     * @brief Returns the font used by the game scene.
     * 
     * @returns A raw pointer to the font.
     */
    Font* GetFont();

    /**
     * @brief Returns the shader program used to render backgrounds to the game scene.
     * 
     * @returns A raw pointer to the shader program.
     */
    ShaderProgram* GetBackgroundShader();

    /**
     * @brief Returns the shader program used to render characters to the game scene.
     * 
     * @returns A raw pointer to the shader program.
     */
    ShaderProgram* GetTextShader();

    /**
     * @brief Returns the window handle used to render the game scene.
     * 
     * @returns A raw pointer to the window.
     */
    Window* GetWindow();

    /**
     * @brief Sets the font used by the game scene.
     * 
     * @param[in] font A raw pointer to the font resource.
     */
    void SetFont(Font* font);

    /**
     * @brief Sets the shader used to render backgrounds to the game scene.
     * 
     * @param[in] shader A raw pointer to the shader program.
     */
    void SetBackgroundShader(ShaderProgram* shader);

    /**
     * @brief Sets the shader program used to render characters to the game scene.
     * 
     * @param[in] shader A raw pointer to the shader program.
     */
    void SetTextShader(ShaderProgram* shader);

    /**
     * @brief Sets the font size this scene renders characters at.
     * 
     * @param[in] font_size The new font size.
     */
    void SetFontSize(uint32_t font_size);

    /// @brief Resizes the window to fit the character map.
    void ResizeToFitCharacterMap();

    /// @brief Resizes the window to fit the window.
    void ResizeToFitWindow();

    /**
     * @brief Returns if the game scene is flagged to be removed.
     * 
     * @returns If the game scene is flagged to be removed.
     */
    bool FlaggedForRemoval() const;

    /// @brief Flags the game scene to be removed.
    void FlagForRemoval();

    /// @brief Unsets the removal flag from the game scene.
    void UnflagForRemoval();

    /// @brief Updates the debugging information for this game scene.
    void UpdateDebugInfo();

  protected:
    /// @brief The name of the game scene.
    std::string name_;
    /// @brief A handle to the window the game scene renders to.
    Window window_;
    /// @brief The background texture rendered behind the game scene.
    Background background_;
    /// @brief The list of characters to render to the game scene.
    CharacterMap character_map_;
    /// @brief The buffer containing vertex data for characters.
    rendering::Buffer text_buffer_;
    /// @brief The buffer containing vertex data for backgrounds.
    rendering::Buffer background_buffer_;
    /// @brief The font used to render characters to the game scene.
    Font* font_;
    /// @brief The shader program used to render backgrounds to the game scene.
    ShaderProgram* background_shader_program_;
    /// @brief The shader program used to render characters to the game scene.
    ShaderProgram* text_shader_program_;
    /// @brief The font size to render characters at, in pixels (px).
    uint32_t font_size_;
    /// @brief A flag to mark this object to be removed.
    bool marked_for_removal_;

    /// @brief Re-sets the projection matrix in the scene's shader, according to the size of the window.
    void ResetProjection();
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
   * @brief Returns the game scene with the given window ID.
   * 
   * @param[in] window_id The window ID the game scene belongs to.
   * @returns A raw pointer to the object, or a null pointer if not found.
   */
  GameScene* GetGameSceneByWindowId(uint32_t window_id);

  /// @brief Updates each game scene in the list.
  void UpdateGameScenes();

  /**
   * @brief Flags the game scene with the given window ID for removal.
   * @details Game scenes are not automatically removed when the user closes a window:
   *          unless a script calls the scene's "preventClose()", the game scene will
   *          close after all objects have been updated.
   * 
   * @param[in] windowId The ID of the window related to the game scene.
   */
  void DoGameSceneEvents(const SDL_Event& event);

  /// @brief Clears all game scenes from the object list.
  void ClearAllGameScenes();

  /// @brief Clears all game scenes that are flagged for removal from the list.
  void ClearFlaggedGameScenes();
}

#endif // ! GAME_SCENE_H