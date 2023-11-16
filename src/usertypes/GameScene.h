/// @author James Holtom

#ifndef GAME_SCENE_H
#define GAME_SCENE_H

#include <memory>
#include <string>
#include <unordered_map>
#include <variant>
#include "CharacterMap.h"
#include "Flaggable.h"
#include "../rendering/Buffer.h"
#include "../utility/SolUtils.h"

namespace term_engine::usertypes {
  class GameScene;
  class GameWindow;

  /// @brief Unique pointer to a game scene.
  typedef std::unique_ptr<GameScene> GameScenePtr;
  /// @brief Used to store a list of game scenes.
  typedef std::unordered_map<std::string, GameScenePtr> GameSceneList;
  /// @brief Used to pass either a GameScene object or it's string index to functions.
  typedef std::variant<GameScene*, std::string> GameSceneVariant;

  /// @brief The name of the default game scene.
  constexpr char DEFAULT_GAME_SCENE_NAME[] = "default";

  /// @brief A list of smart pointers to the game scenes.
  inline GameSceneList game_scene_list;

  /// @brief Manages a game scene, which other game objects can be drawn to and can be used to separate parts of a game.
  class GameScene : public Flaggable {
  public:
    /**
     * @brief Constructs the game scene with the given name.
     * 
     * @param[in] name The name of the scene.
     */
    GameScene(const std::string& name);

    /// @brief Destroys the game scene.
    ~GameScene();

    /**
     * @brief Returns the name of the game scene.
     * 
     * @returns The scene name.
     */
    std::string GetName() const;

    /**
     * @brief Returns the character map so that objects can be drawn to it.
     * 
     * @returns A raw pointer to the character map.
     */
    CharacterMap* GetCharacterMap();

    /**
     * @brief Returns the game window this scene is drawing to.
     * 
     * @returns A raw pointer to the game window.
     */
    GameWindow* GetGameWindow();

    /**
     * @brief Sets the game window this scene draws to.
     * @note Use this function to update the game window as well.
     * 
     * @param[in] game_window The game window to draw to.
     */
    void SetGameWindow(GameWindow* game_window);

    /**
     * @brief Sets the game window this scene draws to.
     * @note This function is used by GameWindow::SetGameScene.
     * 
     * @param[in] game_window The game window to draw to.
     */
    void UpdateGameWindow(GameWindow* game_window);

    /**
     * @brief Returns the Lua function that is called when the scene is loaded.
     * 
     * @returns A raw pointer to the Lua function.
     */
    sol::function* GetOnInit();

    /**
     * @brief Returns the Lua function that is called every frame the scene is active.
     * 
     * @returns A raw pointer to the Lua function.
     */
    sol::function* GetOnLoop();

    /**
     * @brief Returns the Lua function that is called when the scene is unloaded.
     * 
     * @returns A raw pointer to the Lua function.
     */
    sol::function* GetOnQuit();

    /**
     * @brief Sets the Lua function that is called when the scene is loaded.
     * 
     * @param[in] func The Lua function to call.
     */
    void SetOnInit(const sol::function& func);

    /**
     * @brief Sets the Lua function that is called every frame the scene is active.
     * 
     * @param[in] func The Lua function to call.
     */
    void SetOnLoop(const sol::function& func);

    /**
     * @brief Sets the Lua function that is called when the scene is unloaded.
     * 
     * @param[in] func The Lua function to call.
     */
    void SetOnQuit(const sol::function& func);

    /**
     * @brief Calls the Lua function for when the scene is loaded, if set.
     * 
     * @returns The return value of the Lua function.
     */
    bool CallInit();

    /**
     * @brief Calls the Lua function for when the scene is updated, if set.
     * 
     * @param[in] timestep The time since the last frame was run.
     */
    void CallLoop(float timestep);

    /// @brief Calls the Lua function for when the scene is unloaded, if set.
    void CallQuit();

    /// @brief Clears all character data from the scene.
    void ClearMap();

    /// @brief Updates the debugging information for this game scene.
    void UpdateDebugInfo();

  protected:
    /// @brief The name of the game scene.
    std::string name_;
    /// @brief The list of characters to render to the game scene.
    CharacterMap character_map_;
    /// @brief A raw pointer to the game window this scene is drawing to.
    GameWindow* game_window_;
    /// @brief The Lua function to call when this scene is loaded.
    sol::function on_init_;
    /// @brief The Lua function to call every frame the scene is active.
    sol::function on_loop_;
    /// @brief The Lua function to call when this scene is unloaded.
    sol::function on_quit_;
  };

  /**
   * @brief Adds a game scene to the list.
   * 
   * @param[in] name The name of the game scene.
   * @returns A raw pointer to the scene.
   */
  GameScene* AddGameScene(const std::string& name);

  /**
   * @brief Returns the game scene with the given name.
   * 
   * @param[in] name The name of the game scene.
   * @returns A raw pointer to the scene, or a null pointer if not found.
   */
  GameScene* GetGameSceneByName(const std::string& name);

  /// @brief Clears all game scenes that are flagged for removal from the list.
  void ClearFlaggedGameScenes();

  /// @brief Clears all game scenes from the list.
  void ClearAllGameScenes();
}

#endif // ! GAME_SCENE_H