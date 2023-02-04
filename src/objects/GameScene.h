/// @author James Holtom

#ifndef GAME_SCENE_H
#define GAME_SCENE_H

#include <memory>
#include <string>
#include <vector>
#include "BaseObject.h"
#include "../rendering/Background.h"
#include "../rendering/Buffer.h"
#include "../rendering/CharacterMap.h"
#include "../rendering/FontAtlas.h"
#include "../rendering/ShaderProgram.h"
#include "../rendering/Window.h"

namespace term_engine::objects {
  class GameScene;

  typedef std::weak_ptr<GameScene> GameSceneWeakPtr;
  typedef std::shared_ptr<GameScene> GameScenePtr;
  typedef std::pair<std::string, ObjectWeakPtr> GameSceneNamePair;

  constexpr char GAME_SCENE_TYPE[] = "GameScene";

  extern std::vector<GameSceneNamePair> name_list_;

  class GameScene : public BaseObject {
  public:
    /// @brief Constructs the game scene.
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
     * @brief Returns the window handle used to render the game scene.
     * 
     * @returns The window handle.
     */
    rendering::GameWindow* GetWindow();

    /**
     * @brief Returns the font atlas used by the game scene.
     * 
     * @returns The font atlas.
     */
    rendering::FontAtlas* GetFontAtlas();

    /**
     * @brief Returns the background rendered behind the game scene.
     * 
     * @returns The background.
     */
    rendering::Background* GetBackground();

    /**
     * @brief Returns the shader program used to render backgrounds to the game scene.
     * 
     * @returns The shader program.
     */
    rendering::ShaderProgram* GetBackgroundShader();

    /**
     * @brief Returns the shader program used to render characters to the game scene.
     * 
     * @returns The shader program.
     */
    rendering::ShaderProgram* GetTextShader();

    /**
     * @brief Returns the character map so that objects can be drawn to it.
     * 
     * @returns The character map.
     */
    rendering::CharacterMap* GetCharacterMap();

    /// @brief Sets the 'Is Dirty' flag.
    void Dirty();

    /**
     * @brief Returns if the game scene is flagged to be removed.
     * 
     * @returns If the game scene is flagged to be removed.
     */
    bool FlaggedForRemoval() const;

    /// @brief Flags the game scene to be removed.
    void FlagRemoval();

    /// @brief Unsets the removal flag from the game scene.
    void UnflagRemoval();

  private:
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
    rendering::FontAtlas font_atlas_;
    /// @brief The shader program used to render background elements to the game scene.
    rendering::ShaderProgram background_shader_program_;
    /// @brief The shader program used to render text elements to the game scene.
    rendering::ShaderProgram text_shader_program_;
    /// @brief A flag to check if any objects have been modified, and will need to be re-rendered.
    bool is_dirty_;
    /// @brief A flag to mark this game scene to be removed, mainly by _window_close_ events.
    bool marked_for_removal_;
  };

  ///*@brief An object proxy to interact with game scenes.
  class GameSceneProxy : public BaseProxy {
  public:
    /**
     * @brief Constructs the object proxy.
     * 
     * @param[in] object A smart pointer to the object.
     */
    GameSceneProxy(const ObjectPtr& object);

    /// @brief Destroys the object proxy.
    ~GameSceneProxy();

    OBJECT_PROXY_GETTER(GameScene, GetObjectType, std::string)
    OBJECT_PROXY_GETTER(GameScene, GetName, std::string)
    OBJECT_PROXY_GETTER_PTR(GameScene, GetWindow, rendering::GameWindow)
    OBJECT_PROXY_GETTER_PTR(GameScene, GetFontAtlas, rendering::FontAtlas)
    OBJECT_PROXY_GETTER_PTR(GameScene, GetBackground, rendering::Background)
    OBJECT_PROXY_GETTER_PTR(GameScene, GetBackgroundShader, rendering::ShaderProgram)
    OBJECT_PROXY_GETTER_PTR(GameScene, GetTextShader, rendering::ShaderProgram)
    OBJECT_PROXY_GETTER_PTR(GameScene, GetCharacterMap, rendering::CharacterMap)
    OBJECT_PROXY_CALLER(GameScene, FlagRemoval)
    OBJECT_PROXY_CALLER(GameScene, UnflagRemoval)
  };

  /**
   * @brief Adds a game scene to the list.
   * 
   * @param[in] name The name of the game scene.
   * @returns A proxy to the object.
   */
  GameSceneProxy AddGameScene(const std::string& name);

  ObjectWeakPtr GetGameSceneByName(const std::string& name);

  void MarkGameSceneForRemoval(const Uint32& windowId);

  /// @brief Clears all game scenes from the object list.
  void ClearAllGameScenes();

  /// @brief Clears all game scenes that are flagged for removal from the object list.
  void ClearFlaggedGameScenes();
}

#endif // ! GAME_SCENE_H