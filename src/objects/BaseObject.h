#ifndef BASE_OBJECT_H
#define BASE_OBJECT_H

#include <forward_list>
#include <memory>
#include <string>

namespace term_engine::objects {
  class BaseObject;

  /// @brief Smart pointer to an object.
  typedef std::shared_ptr<BaseObject> ObjectPtr;
  /// @brief Used to store a list of objects.
  typedef std::forward_list<ObjectPtr> ObjectList;

  enum ObjectSortPriority { EVENT_LISTENER = 0, TIMED_FUNCTION = 1, OBJECT = 2, AUDIO = 3, GAME_SCENE = 4 };

  /// @brief The base object, on which other game objects are derived from.
  class BaseObject {
  public:
    /// @brief Constructs the base object.
    BaseObject();

    /**
     * @brief Default logic to destroy a base object.
     * @details Required to be virtual to enforce only the construction of derived classes.
     */
    virtual ~BaseObject() {}

    /// @brief Updates the object every frame.
    virtual void Update() = 0;

    /**
     * @brief Returns the type of the object.
     * 
     * @return The object type.
     */
    virtual std::string GetObjectType() const = 0;

    /**
     * @brief Returns the object ID.
     * 
     * @return The object ID.
     */
    size_t GetObjectId() const;

    /**
     * @brief Returns if the object is active.
     * 
     * @returns If the object is active.
     */
    bool IsActive() const;

    /**
     * @brief Sets if the object is active.
     * 
     * @param[in] flag The value to set.
     */
    void SetActive(const bool& flag);
    
    /// @brief Has an object been added to the list, which needs to be sorted?
    static bool is_list_dirty_;
    /// @brief The list of objects.
    static ObjectList object_list_;

    /**
     * @brief Sorts the objects into order.
     * @details The order of sorted objects follows the _ObjectSortPriority_ enumeration.
     */
    static void Sort();

    /**
     * @brief Removes an object from the list.
     * 
     * @param[in] obj A smart pointer to the object.
     */
    static void Remove(ObjectPtr& object);

    /// @brief Removes all objects in the list.
    static void CleanUp();

  protected:
    /// @brief The ID of the object.
    std::size_t object_id_;
    /// @brief Is the object active? (i.e. Is the object being rendered and acted on?)
    bool is_active_;
    /// @brief Represents the ID to use for the next object.
    static std::size_t object_next_id_;

    /**
     * @brief Returns the list priority for this object.
     * @details This is used to sort the list of objects before updating.
     * 
     * @returns The priority for this object.
     */
    virtual ObjectSortPriority GetListPriority() const = 0;
  };
}

#endif // ! BASE_OBJECT_H