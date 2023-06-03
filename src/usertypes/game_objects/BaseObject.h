/// @author James Holtom

#ifndef BASE_OBJECT_H
#define BASE_OBJECT_H

#include <forward_list>
#include <memory>
#include <string>
#include "../../utility/LogUtils.h"

namespace term_engine::usertypes {
  class BaseObject;

  /// @brief Shared pointer to an object.
  typedef std::shared_ptr<BaseObject> ObjectPtr;
  /// @brief Weak pointer to an object.
  typedef std::weak_ptr<BaseObject> ObjectWeakPtr;
  /// @brief Used to store a list of objects.
  typedef std::forward_list<ObjectPtr> ObjectList;

  /// @brief Defines the sorting priorities for each type of object in the list.
  enum ObjectSortPriority { EVENT_LISTENER = 0, TIMED_FUNCTION = 1, OBJECT = 2, AUDIO = 3 };

  /// @brief Has an object been added to the list, which needs to be sorted?
  inline bool is_object_list_dirty_ = false;
  /// @brief The list of objects.
  inline ObjectList object_list_;

  /// @brief The base object, on which other game objects are derived from.
  class BaseObject {
  public:
    /// @brief Constructs the base object.
    BaseObject();

    /**
     * @brief Returns the type of object.
     * 
     * @returns The object type.
     */
    virtual std::string GetObjectType() const = 0;

    /// @brief Updates the object every frame.
    virtual void Update(uint64_t timestep) = 0;

    /**
     * @brief Returns the list priority for this object.
     * @details This is used to sort the list of objects before updating.
     * 
     * @returns The priority for this object.
     */
    virtual ObjectSortPriority GetListPriority() const = 0;

    /**
     * @brief Returns the object ID.
     * 
     * @returns The object ID.
     */
    uint64_t GetObjectId() const;

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
    void SetActive(bool flag);

    /**
     * @brief Returns if the object is flagged to be removed.
     * 
     * @returns If the object is flagged to be removed.
     */
    bool FlaggedForRemoval() const;

    /// @brief Flags the object to be removed.
    void FlagForRemoval();

    /// @brief Unsets the removal flag from the object.
    void UnflagForRemoval();

    /// @brief Updates the debugging information for this object.
    virtual void UpdateDebugInfo() const = 0;

    static uint64_t GetNextId();

  protected:
    /// @brief The ID of the object.
    uint64_t object_id_;
    /// @brief Is the object active? (i.e. Is the object being rendered and acted on?)
    bool is_active_;
    /// @brief A flag to mark this object to be removed.
    bool marked_for_removal_;
    /// @brief Represents the ID to use for the next object.
    static uint64_t object_next_id_;
  };

  /**
   * @brief Sorts the objects into order.
   * @details The order of sorted objects follows the _ObjectSortPriority_ enumeration.
   */
  void SortObjects();

  /// @brief Runs the `Update()` function for each object in the list.
  void UpdateObjects(uint64_t timestep);

  /// @brief Removes all objects in the list.
  void CleanUpObjects();

  /// @brief Clears all objects that are flagged for removal from the object list.
  void ClearFlaggedObjects();
}

#endif // ! BASE_OBJECT_H