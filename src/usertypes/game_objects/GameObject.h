/// @author James Holtom

#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <string>
#include "BaseObject.h"
#include "../Character.h"
#include "../CharacterMap.h"
#include "../GameScene.h"
#include "../resources/Animation.h"
#include "../../utility/GLUtils.h"
#include "../../utility/SolUtils.h"

namespace term_engine::usertypes {
  /// @brief The type name for GameObjects.
  constexpr char GAME_OBJECT_TYPE[] = "GameObject";

  /// @brief Used to represent a game object, that is rendered to the screen.
  class GameObject : public BaseObject {
  public:
    /**
     * @brief Constructs the object with the given parameters.
     * 
     * @param[in] game_scene The game scene the object belongs to.
     * @param[in] position   The position of the object in the window.
     * @param[in] size       The size of the object, i.e. how many rows & columns.
     */
    GameObject(GameScene* game_scene, const glm::ivec2& position, const glm::ivec2& size);

    /**
     * @brief Copy-constructs the object from an existing object.
     * 
     * @param[in] game_scene  The game scene the object belongs to.
     * @param[in] object      The object to copy from.
     */
    GameObject(GameScene* game_scene, GameObject* object);

    /// @brief Destroys the object.
    ~GameObject();

    /**
     * @brief Updates the object.
     * 
     * @param[in] timestep The time since the last update, in milliseconds (ms).
     */
    void Update(uint64_t timestep);

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
     * @brief Returns a raw pointer to the game scene this object belongs to.
     * 
     * @returns The raw pointer to the game scene.
     */
    GameScene* GetGameScene() const;

    /**
     * @brief Returns the Z-layer this object occupies.
     * 
     * @returns The Z-layer this object occupies.
     */
    int32_t GetLayer() const;

    /**
     * @brief Returns if the mouse is currently hovering over the object.
     * 
     * @returns If the mouse is hovering over the object.
     */
    bool IsHoveringOver() const;

    /**
     * @brief Returns the position of the object.
     * 
     * @returns The position of the object.
     */
    glm::ivec2& GetPosition();

    /**
     * @brief Returns the object's character data.
     * 
     * @returns The character data.
     */
    CharacterMap& GetCharacterMap();

    /**
     * @brief Returns the animation state of the object.
     * 
     * @returns The animation state for the object.
     */
    AnimationState& GetAnimation();

    /**
     * @brief Sets the Z-layer this object occupies.
     * 
     * @param[in] layer The Z-layer to set.
     */
    void SetLayer(int32_t layer);

    /**
     * @brief Sets the position of the object.
     * 
     * @param[in] position The position to move the object to.
     */
    void SetPosition(const glm::ivec2& position);

    /**
     * @brief Sets the character data for the object.
     * 
     * @param[in] data The character data.
     */
    void SetCharacterMap(const CharacterMap& data);

    /**
     * @brief Applies the results of a user-defined function on the object data.
     * 
     * @param func The user-defined function to apply to the object data.
     */
    void Set(const sol::function& func);

    /**
     * @brief Moves the object to the given game scene.
     * 
     * @param[in] name The name of the target game scene.
     */
    void MoveToGameScene(const std::string& name);

    /**
     * @brief Copies the object to the given game scene.
     * 
     * @param[in] name The name of the target game scene.
     * @returns A raw pointer to the copied object.
     */
    GameObject* CopyToGameScene(const std::string& name);

    /// @brief Updates the debugging information for this object.
    void UpdateDebugInfo() const;

  protected:
    /// @brief The Z-layer this object occupies.
    int32_t layer_;
    /// @brief Is the mouse hovering over the object?
    bool is_hovering_;
    /// @brief The top-left position of the object.
    glm::ivec2 position_;
    /// @brief The character data that will be copied to the buffer when rendered.
    CharacterMap data_;
    /// @brief The animation state of the object.
    AnimationState animation_state_;
    /// @brief The game scene this object renders to.
    GameScene* game_scene_;
  };

  /**
   * @brief Adds a game object to the list.
   * 
   * @param[in] position   The position of the game object.
   * @param[in] size       The size of the game object, in rows & columns.
   * @param[in] game_scene The game scene the game object belongs to.
   * @returns A raw pointer to the object.
   */
  GameObject* AddGameObjectToScene(const glm::ivec2& position, const glm::ivec2& size, GameSceneVariant game_scene);

  /**
   * @brief Adds a game object for the default game scene to the list.
   * 
   * @param[in] position   The position of the game object.
   * @param[in] size       The size of the game object, in rows & columns.
   * @returns A raw pointer to the object.
   */
  GameObject* AddGameObject(const glm::ivec2& position, const glm::ivec2& size);

  /**
   * @brief Clears all game objects belonging to a game scene.
   * 
   * @param[in] name The name of the game scene.
   */
  void ClearGameObjectsByGameScene(const std::string& name);

  /**
   * @brief Returns the game object with the given ID.
   * 
   * @param[in] id The ID of the game object.
   * @returns A raw pointer to the game object, or a null pointer if not found.
   */
  GameObject* GetGameObjectById(uint64_t id);
}

#endif // ! GAME_OBJECT_H
