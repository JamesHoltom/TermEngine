/// @author James Holtom

#ifndef EVENT_LISTENER_H
#define EVENT_LISTENER_H

#include "BaseObject.h"
#include "GameScene.h"
#include "../utility/SolUtils.h"

namespace term_engine::objects {
  /// @brief The type name for EventListeners.
  constexpr char EVENT_LISTENER_TYPE[] = "EventListener";

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
    EventListener(GameScene* game_scene, const std::string& type, const sol::function callback);

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

  protected:
    /// @brief The type of event being listened to.
    std::string type_;
    /// @brief The callback function to call when the event is fired.
    sol::function callback_;
    /// @brief The game scene this event listener listens to.
    GameScene* game_scene_;
  };

  /**
   * @brief Adds an event listener to the list.
   * 
   * @param[in] type      The type of event to listen to.
   * @param[in] name      The name of the game scene.
   * @param[in] callback  The user-defined callback function to trigger when an event is received.
   * @returns A raw pointer to the object.
   */
  EventListener* AddEventListenerToScene(const std::string& type, const std::string& name, const sol::function callback);

  /**
   * @brief Adds an event listener for the default game scene to the list.
   * 
   * @param[in] type      The type of event to listen to.
   * @param[in] callback  The user-defined callback function to trigger when an event is received.
   * @returns A raw pointer to the object.
   */
  EventListener* AddEventListener(const std::string& type, const sol::function callback);

  /// @brief Clears all event listeners from the object list.
  void ClearAllEventListeners();

  /**
   * @brief Clears all event listeners belonging to a game scene.
   * 
   * @param[in] name The name of the game scene.
   */
  void ClearEventListenersByGameScene(const std::string& name);
}

#endif // ! EVENT_LISTENER_H
