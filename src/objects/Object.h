/// @author James Holtom

#ifndef OBJECT_H
#define OBJECT_H

#include <functional>
#include <memory>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include "../rendering/Character.h"
#include "../utility/SolUtils.h"

namespace objects {
  class Object;

  /// @brief Smart pointer to an object.
  typedef std::shared_ptr<Object> ObjectPtr;
  /// @brief Used to store a list of objects.
  typedef std::vector<ObjectPtr> ObjectList;

  /**
   * @brief Used to represent a game object, that is rendered to the screen.
   * @details Other attributes can be added at the script level; this structure only cares about rendering characters to the screen.
   */
  class Object {
  public:
    /**
     * @brief Constructs the object with the given parameters.
     * 
     * @param[in] position The position of the object in the window.
     * @param[in] size     The size of the object, i.e. how many rows & columns.
     */
    Object(const glm::ivec2& position, const glm::ivec2& size);

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
     * @brief Returns if the object is active.
     * 
     * @returns If the object is active.
     */
    bool IsActive() const;

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
     * @brief Sets if the object is active.
     * 
     * @param[in] flag The value to set.
     */
    void SetActive(const bool& flag);

    /**
     * @brief Applies the results of a user-defined function on the object data.
     * 
     * @param func The user-defined function to apply to the object data.
     */
    void Set(const sol::function& func);

    /**
     * @brief Returns if 2 objects share the same ID.
     * 
     * @param[in] rhs The object to compare this object to.
     * @returns If the 2 objects are the same.
     */
    bool operator==(const Object& rhs) const
    {
      return object_id_ == rhs.object_id_;
    }

    /**
     * @brief Adds an object to the list.
     * 
     * @param[in] position The position of the object.
     * @param[in] size     The size of the object, in rows & columns.
     * @returns A smart pointer to the object if it was added to the list, or a null pointer if it failed.
     */
    static ObjectPtr& Add(const glm::ivec2& position, const glm::ivec2& size);

    /**
     * @brief Adds an object to the list, using Lua's colon syntax (i.e. Object:new(...)).
     * 
     * @param[in] self     Reference to the object. Unused.
     * @param[in] position The position of the object.
     * @param[in] size     The size of the object, in rows & columns.
     * @returns A smart pointer to the object if it was added to the list, or a null pointer if it failed.
     */
    static ObjectPtr& AddSelf(sol::object self, const glm::ivec2& position, const glm::ivec2& size);

    /**
     * @brief Removes an object from the list.
     * 
     * @param[in] obj A smart pointer to the object.
     */
    static void Remove(ObjectPtr& obj);

    /**
     * @brief Retrieves the list of objects.
     * 
     * @returns The list of objects.
     */
    static ObjectList& GetList();

    /**
     * @brief Returns the number of objects.
     * 
     * @returns The number of objects.
     */
    static int Count();
    
    /// @brief Removes all objects in the list.
    static void CleanUp();

    /**
     * @brief Returns if the 'Is Dirty' flag is set, and there are objects that need to be re-rendered.
     * 
     * @returns If the 'Is Dirty' flag is set.
     */
    static bool IsDirty();

    /// @brief Sets the 'Is Dirty' flag.
    static void Dirty();

    /// @brief Unsets the 'Is Dirty' flag.
    static void Clean();

  private:
    /// @brief The top-left position of the object.
    glm::ivec2 position_;
    /// @brief The size of the object, in rows & columns.
    glm::ivec2 size_;
    /// @brief The character parameters that will copied to the buffer when rendered.
    rendering::CharacterData data_;
    /// @brief The ID of the object.
    int object_id_;
    /// @brief Is the object active? (i.e. Is the object being rendered and acted on?)
    bool is_active_;
    /// @brief Represents the ID to use for the next object.
    static int object_next_id_;
    /// @brief A flag to check if any objects have been modified, and will need to be re-rendered.
    static bool is_dirty_;
    /// @brief The list of objects.
    static ObjectList object_list_;
  };
}

#endif // ! OBJECT_H
