/// @author James Holtom

#ifndef EVENT_LISTENER_H
#define EVENT_LISTENER_H

#include "BaseObject.h"
#include "GameScene.h"
#include "../utility/SolUtils.h"

namespace term_engine::objects {
  /// @brief Represents a listener which triggers a user-defined callback function when a particular type of event is received.
  class EventListener : public BaseObject {
  public:
    /**
     * @brief Constructs the event listener with the given callback.
     * 
     * @param[in] game_scene The game scene the event listener listens to.
     * @param[in] type       The type of event to listen to.
     * @param[in] callback   The user-defined callback function to trigger when an event is received.
     */
    EventListener(const GameSceneWeakPtr& game_scene, const std::string& type, const sol::function callback);

    /// @brief Destroys the event listener.
    ~EventListener();
    
    /// @brief Updates the event listener.
    void Update();

    /**
     * @brief Returns the type of the object.
     * 
     * @return The object type.
     */
    std::string GetObjectType() const;

    /**
     * @brief Returns the type of event this listener will trigger for.
     * 
     * @returns The type of event being listened for.
     */
    std::string GetListenerType() const;

    /**
     * @brief Executes the callback function.
     * 
     * @param[in] data The data to pass to the callback function.
     */
    void Trigger(const sol::table& data);

    /**
     * @brief Adds an event listener to the list.
     * 
     * @returns A smart pointer to the event listener if it was added to the list, or a null pointer if it failed.
     */
    static ObjectPtr& Add(const GameScenePtr& game_scene, const std::string& type, const sol::function callback);

    /// @brief Clears all event listeners from the object list.
    static void ClearAll();

  protected:
    /// @brief The type of event being listened to.
    std::string type_;
    /// @brief The callback function to call when the event is fired.
    sol::function callback_;
    /// @brief The game scene this event listener listens to.
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

#endif // ! EVENT_LISTENER_H
