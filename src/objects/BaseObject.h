/// @author James Holtom

#ifndef BASE_OBJECT_H
#define BASE_OBJECT_H

#include <forward_list>
#include <memory>
#include <optional>
#include <string>
#include "../utility/SpdlogUtils.h"

#define OBJECT_PROXY_GETTER(P, F, R) \
  std::optional<R> F() const \
  { \
    if (ptr_.expired()) \
    { \
      utility::logger->warn("Cannot return value from a removed object!"); \
      return std::nullopt; \
    } \
    return std::dynamic_pointer_cast<P>(ptr_.lock())->F(); \
  }

#define OBJECT_PROXY_GETTER_PTR(P, F, R) \
  R* F() const \
  { \
    if (ptr_.expired()) \
    { \
      utility::logger->warn("Cannot return value from a removed object!"); \
      return nullptr; \
    } \
    return std::dynamic_pointer_cast<P>(ptr_.lock())->F(); \
  }

#define OBJECT_PROXY_SETTER(P, F, S) \
  void F(const S& value) const \
  { \
    if (ptr_.expired()) \
    { \
      utility::logger->warn("Cannot set value on a removed object!"); \
      return; \
    } \
    std::dynamic_pointer_cast<P>(ptr_.lock())->F(value); \
  }

#define OBJECT_PROXY_CALLER(P, F) \
  void F() const \
  { \
    if (ptr_.expired()) \
    { \
      utility::logger->warn("Cannot call function for a removed object!"); \
      return; \
    } \
    std::dynamic_pointer_cast<P>(ptr_.lock())->F(); \
  }

#define OBJECT_PROXY_CALLER_WITH_PARAM(P, F, T) \
  void F(const T& value) const \
  { \
    if (ptr_.expired()) \
    { \
      utility::logger->warn("Cannot call function for a removed object!"); \
      return; \
    } \
    std::dynamic_pointer_cast<P>(ptr_.lock())->F(value); \
  }

#define OBJECT_PROXY_CALLER_WITH_PARAM_AND_RETURN(P, F, R, T) \
  std::optional<R> F(const T& value) const \
  { \
    if (ptr_.expired()) \
    { \
      utility::logger->warn("Cannot call function for a removed object!"); \
      return std::nullopt; \
    } \
    return std::dynamic_pointer_cast<P>(ptr_.lock())->F(value); \
  }

namespace term_engine::objects {
  class BaseObject;

  /// @brief Shared pointer to an object.
  typedef std::shared_ptr<BaseObject> ObjectPtr;
  /// @brief Weak pointer to an object.
  typedef std::weak_ptr<BaseObject> ObjectWeakPtr;
  /// @brief Used to store a list of objects.
  typedef std::forward_list<ObjectPtr> ObjectList;

  enum ObjectSortPriority { EVENT_LISTENER = 0, TIMED_FUNCTION = 1, OBJECT = 2, AUDIO = 3, GAME_SCENE = 4 };

  /// @brief Has an object been added to the list, which needs to be sorted?
  extern bool is_object_list_dirty_;
  /// @brief The list of objects.
  extern ObjectList object_list_;

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
     * @brief Returns the type of object.
     * 
     * @returns The object type.
     */
    virtual std::string GetObjectType() const = 0;

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

  protected:
    /// @brief The ID of the object.
    std::size_t object_id_;
    /// @brief Is the object active? (i.e. Is the object being rendered and acted on?)
    bool is_active_;
    /// @brief Represents the ID to use for the next object.
    static std::size_t object_next_id_;
  };

  /**
   * @brief The base object proxy, on which other object proxies derive from.
   * @remarks The object proxy is set up as a usertype in Sol, rather than the object itself.
   * This is because the lifetime of objects is handled in C++, not Lua.
   * If the object the proxy refers to is removed, calling any member function will print an error message and return nil, if required.
   */
  class BaseProxy {
  public:
    /**
     * @brief Constructs the object proxy.
     * 
     * @param[in] object A smart pointer to the object.
     */
    BaseProxy(const ObjectPtr& object);

    /**
     * @brief Default logic to destroy an object proxy.
     * @details Required to be virtual to enforce only the construction of derived classes.
     */
    virtual ~BaseProxy() {}

    /**
     * @brief Returns the type of object.
     * 
     * @returns The object type, or nil if the object is expired.
     */
    virtual std::optional<std::string> GetObjectType() const = 0;

    OBJECT_PROXY_GETTER(BaseObject, GetObjectId, size_t)
    OBJECT_PROXY_GETTER(BaseObject, IsActive, bool)
    OBJECT_PROXY_SETTER(BaseObject, SetActive, bool)

    /**
     * @brief Returns if the object is expired.
     * 
     * @returns If the object is expired.
     */
    bool IsExpired() const;

    /**
     * @brief Releases the object the proxy points to.
     * @remarks This causes all other object proxies to expire, that point to this object.
     */
    void Release() const;

  protected:
    /// @brief Smart pointer to the object.
    ObjectWeakPtr ptr_;
  };

  /**
   * @brief Sorts the objects into order.
   * @details The order of sorted objects follows the _ObjectSortPriority_ enumeration.
   */
  void SortObjects();

  /**
   * @brief Removes an object from the list.
   * 
   * @param[in] id The ID of the object.
   */
  void RemoveObject(const size_t& id);

  /// @brief Removes all objects in the list.
  void CleanUpObjects();
}

#endif // ! BASE_OBJECT_H