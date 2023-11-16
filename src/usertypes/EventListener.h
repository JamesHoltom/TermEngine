/// @author James Holtom

#ifndef EVENT_LISTENER_H
#define EVENT_LISTENER_H

#include <forward_list>
#include <memory>
#include <string>
#include "Flaggable.h"
#include "../events/Listener.h"
#include "../utility/SolUtils.h"

namespace term_engine::usertypes {
  class EventListener;

  /// @brief Unique pointer to a game scene.
  typedef std::unique_ptr<EventListener> EventListenerPtr;
  /// @brief Used to store a list of game scenes.
  typedef std::forward_list<EventListenerPtr> EventListenerList;

  /// @brief A list of raw pointers to the game scenes.
  inline EventListenerList listener_list;

  /// @brief Represents a listener which triggers a user-defined callback function when a particular type of event is received.
  class EventListener : public Flaggable {
  public:
    /**
     * @brief Constructs the event listener with the given callback.
     * 
     * @param[in] type       The type of event to listen to.
     * @param[in] callback   The user-defined callback function to trigger when an event is received.
     */
    EventListener(const std::string& type, const sol::protected_function callback);

    /// @brief Destroys the event listener.
    ~EventListener();
    
    /**
     * @brief Updates the event listener with events from SDL.
     * 
     * @param[in] event The SDL event to update the listener with.
     */
    void DoEvents(const events::Event& event);

    /**
     * @brief Returns the listener ID.
     * 
     * @returns The listener ID.
     */
    uint64_t GetListenerId() const;

    /**
     * @brief Returns the type of event this listener will trigger for.
     * 
     * @returns The type of event being listened for.
     */
    std::string GetListenerType() const;

    /**
     * @brief Returns the number of times this listener has been triggered.
     * 
     * @returns The number of times this listener has been triggered.
     */
    uint32_t GetTimesFired() const;

    /**
     * @brief Returns if the listener is active.
     * 
     * @returns If the listener is active.
     */
    bool IsActive() const;

    /**
     * @brief Sets if the listener is active.
     * 
     * @param[in] flag The value to set.
     */
    void SetActive(bool flag);

    /**
     * @brief Executes the callback function.
     * 
     * @param[in] data The data to pass to the callback function.
     */
    void Trigger(const sol::table& data);

    /// @brief Updates the debugging information for this object.
    void UpdateDebugInfo() const;

  protected:
    /// @brief The ID of the listener.
    uint64_t listener_id_;
    /// @brief The type of event being listened to.
    std::string type_;
    /// @brief The callback function to call when the event is fired.
    sol::protected_function callback_;
    /// @brief The number of times the listener has been fired.
    uint32_t times_fired_;
    /// @brief Is the listener active? (i.e. Is the listener triggering when events are raised?)
    bool is_active_;
    /// @brief Represents the ID to use for the next listener.
    static uint64_t listener_next_id_;
  };

  /**
   * @brief Adds an event listener for the default game scene to the list.
   * 
   * @param[in] type      The type of event to listen to.
   * @param[in] callback  The user-defined callback function to trigger when an event is received.
   * @returns A raw pointer to the object.
   */
  EventListener* AddEventListener(const std::string& type, const sol::protected_function callback);

  /**
   * @brief Returns the event listener with the given ID.
   * 
   * @param[in] id The ID of the event listener to get.
   * @returns A raw pointer to the event listener, or a null pointer if not found.
   */
  EventListener* GetEventListenerById(uint64_t id);

  /**
   * @brief Removes the event listener with the given ID from the list.
   * 
   * @param[in] id The ID of the event listener to remove.
   */
  void RemoveEventListener(uint64_t id);

  /**
   * @brief Removes the given event listener from the list.
   * 
   * @param[in] ptr Raw pointer to the event listener.
   */
  void RemoveEventListener(EventListener* ptr);

  /// @brief Clears all listeners from the list.
  void ClearAllEventListeners();

  /// @brief Clears all listeners that are flagged for removal from the list.
  void ClearFlaggedEventListeners();
}

#endif // ! EVENT_LISTENER_H
