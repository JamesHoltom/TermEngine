/// @author James Holtom

#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <functional>
#include <string>
#include "BaseObject.h"
#include "GameScene.h"
#include "../rendering/Character.h"
#include "../utility/GLUtils.h"
#include "../utility/SolUtils.h"

namespace term_engine::objects {
  class GameObjectProxy;

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
    GameObject(const GameSceneWeakPtr& game_scene, const glm::ivec2& position, const glm::ivec2& size);

    /**
     * @brief Copy-constructs the object from an existing object.
     * 
     * @param[in] game_scene The game scene the object belongs to.
     * @param[in] object 
     */
    GameObject(const GameSceneWeakPtr& game_scene, const GameObject* object);

    /// @brief Destroys the object.
    ~GameObject();

    /// @brief Updates the object.
    void Update();

    /**
     * @brief Returns the type of the object.
     * 
     * @return The object type.
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
     * @brief Returns a weak pointer to the game scene this object belongs to.
     * 
     * @returns The weak pointer to the game scene.
     */
    GameSceneWeakPtr GetGameScene() const;

    /**
     * @brief Returns the position of the object.
     * 
     * @returns The position of the object.
     */
    glm::ivec2 GetPosition() const;

    /**
     * @brief Returns the size of the object.
     * 
     * @returns The size of the object.
     */
    glm::ivec2 GetSize() const;

    /**
     * @brief Returns a reference to the object's data.
     * 
     * @returns The data within the object.
     */
    rendering::CharacterData* GetData();

    /**
     * @brief Sets the position of the object.
     * 
     * @param[in] position The position to move the object to.
     */
    void SetPosition(const glm::ivec2& position);

    /**
     * @brief Sets the size of the object.
     * 
     * @param[in] size The new size of the object.
     */
    void SetSize(const glm::ivec2& size);

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
     * @returns The copied object.
     */
    GameObjectProxy CopyToGameScene(const std::string& name);

  private:
    /// @brief The top-left position of the object.
    glm::ivec2 position_;
    /// @brief The size of the object, in rows & columns.
    glm::ivec2 size_;
    /// @brief Has the object been modified, and requires updating?
    bool is_dirty_;
    /// @brief The character parameters that will copied to the buffer when rendered.
    rendering::CharacterData data_;
    /// @brief The game scene this object renders to.
    GameSceneWeakPtr game_scene_;
  };

  /// @brief An object proxy to interact with game objects.
  class GameObjectProxy : public BaseProxy {
  public:
    /**
     * @brief Constructs the object proxy.
     * 
     * @param[in] object A smart pointer to the object.
     */
    GameObjectProxy(const ObjectPtr& object);

    /// @brief Destroys the object proxy.
    ~GameObjectProxy();

    OBJECT_PROXY_GETTER(GameObject, GetObjectType, std::string)
    OBJECT_PROXY_GETTER(GameObject, GetPosition, glm::ivec2)
    OBJECT_PROXY_GETTER(GameObject, GetSize, glm::ivec2)
    OBJECT_PROXY_GETTER_PTR(GameObject, GetData, rendering::CharacterData)
    OBJECT_PROXY_SETTER(GameObject, SetPosition, glm::ivec2)
    OBJECT_PROXY_SETTER(GameObject, SetSize, glm::ivec2)
    OBJECT_PROXY_CALLER_WITH_PARAM(GameObject, Set, sol::function)
    OBJECT_PROXY_CALLER_WITH_PARAM(GameObject, MoveToGameScene, std::string)
    OBJECT_PROXY_CALLER_WITH_PARAM_AND_RETURN(GameObject, CopyToGameScene, GameObjectProxy, std::string)
  };

  /**
   * @brief Adds a game object to the list.
   * 
   * @param[in] position   The position of the game object.
   * @param[in] size       The size of the game object, in rows & columns.
   * @param[in] name       The name of the game scene the game object belongs to.
   * @returns A proxy to the object.
   */
  GameObjectProxy AddGameObjectToScene(const glm::ivec2& position, const glm::ivec2& size, const std::string& name);

  /**
   * @brief Adds a game object for the default game scene to the list.
   * 
   * @param[in] position   The position of the game object.
   * @param[in] size       The size of the game object, in rows & columns.
   * @returns A proxy to the object.
   */
  GameObjectProxy AddGameObject(const glm::ivec2& position, const glm::ivec2& size);

  /// @brief Clears all game objects from the object list.
  void ClearAllGameObjects();

  /**
   * @brief Clears all game objects belonging to a game scene.
   * 
   * @param[in] name The name of the game scene.
   */
  void ClearObjectsByGameScene(const std::string& name);
}

#endif // ! GAME_OBJECT_H
