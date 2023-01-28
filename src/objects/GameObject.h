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
  class GameObject;

  typedef std::shared_ptr<GameObject> GameObjectPtr;

  /**
   * @brief Used to represent a game object, that is rendered to the screen.
   * @details Other attributes can be added at the script level; this structure only cares about rendering characters to the screen.
   */
  class GameObject : public BaseObject {
  public:
    /**
     * @brief Constructs the object with the given parameters.
     * 
     * @param[in] game_scene The game scene the object renders to.
     * @param[in] position   The position of the object in the window.
     * @param[in] size       The size of the object, i.e. how many rows & columns.
     */
    GameObject(const GameSceneWeakPtr& game_scene, const glm::ivec2& position, const glm::ivec2& size);

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
    rendering::CharacterData& GetData();

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

    /// @brief Marks that the object has been modified, and will need to be updated in the list.
    void Dirty();

    /**
     * @brief Adds an object to the list.
     * 
     * @param[in] game_scene The game scene the object renders to.
     * @param[in] position   The position of the object.
     * @param[in] size       The size of the object, in rows & columns.
     * @returns A smart pointer to the object if it was added to the list, or a null pointer if it failed.
     */
    static GameObjectPtr& Add(const GameScenePtr& game_scene, const glm::ivec2& position, const glm::ivec2& size);

    /// @brief Clears all objects from the object list.
    static void ClearAll();

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

    /**
     * @brief Returns the list priority for this object.
     * @details This is used to sort the list of objects before updating.
     * 
     * @returns The priority for this object.
     */
    ObjectSortPriority GetListPriority() const;
  };
}

#endif // ! GAME_OBJECT_H
