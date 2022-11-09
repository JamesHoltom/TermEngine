/// @author James Holtom

#ifndef LISTENER_H
#define LISTENER_H

#include <memory>
#include <unordered_map>
#include <vector>
#include "../utility/SDLUtils.h"
#include "../utility/SolUtils.h"

namespace term_engine::events {
  class EventListener;

  /// @brief Smart pointer to an event listener.
  typedef std::shared_ptr<EventListener> ListenerPtr;
  /// @brief Used to store a list of listeners for a particular type of event.
  typedef std::vector<ListenerPtr> ListenerList;
  /// @brief Used to store a list of event types, and their list of listeners.
  typedef std::unordered_map<std::string, ListenerList> ListenerMap;

  /// @brief Represents a listener which triggers a user-defined callback function when a particular type of event is received.
  class EventListener {
  public:
    /**
     * @brief Constructs the event listener with the given callback.
     * 
     * @param[in] type The type of event to listen to.
     * @param[in] func The user-defined callback function to trigger when an event is received.
     */
    EventListener(const std::string& type, const sol::function func);
    
    /**
     * @brief Returns the type of event this listener will trigger for.
     * 
     * @returns The type of event being listened for.
     */
    std::string GetType() const;

    /**
     * @brief Returns if the listener is active.
     * 
     * @returns Is the listener active?
     */
    bool IsActive() const;

    /**
     * @brief Sets if the listener is active and will trigger when events are received.
     * 
     * @param[in] active Flag to (de)activate the event listener.
     */
    void SetActive(const bool& active);

    /**
     * @brief Executes the callback function.
     * 
     * @param[in] data The data to pass to the callback function.
     */
    void Trigger(const sol::table& data);

    /**
     * @brief Adds a listener for a given event type to the list.
     * 
     * @param[in] type The type of event to listen for.
     * @param[in] func The user-defined callback function to execute when triggered.
     * @returns A smart pointer to the event listener if it was added to the list, or a null pointer if it failed.
     */
    static ListenerPtr& Add(const std::string& type, const sol::function func);

    /**
     * @brief Adds a listener for a given event type to the list, using Lua's colon syntax (i.e. EventListener:new(...)).
     * 
     * @param[in] self Reference to the object. Unused.
     * @param[in] type The type of event to listen for.
     * @param[in] func The user-defined callback function to execute when triggered.
     * @returns A smart pointer to the event listener if it was added to the list, or a null pointer if it failed.
     */
    static ListenerPtr& AddSelf(sol::object self, const std::string& type, const sol::function func);

    /**
     * @brief Removes a listener from the list.
     * 
     * @param[in] event A smart pointer to the event listener.
     */
    static void Remove(ListenerPtr& event);

    /**
     * @brief Removes all listeners for the given event type from the list.
     * 
     * @param[in] type The type of event to clear listeners from.
     */
    static void Clear(const std::string& type);

    /**
     * @brief Returns the number of listeners for the given type.
     * 
     * @param[in] type The type of event to count listeners for.
     * @returns The amount of listeners.
     */
    static int Count(const std::string& type);

    /**
     * @brief Returns the list of event types with listeners attached.
     * 
     * @returns The list of event types.
     */
    static std::vector<std::string> GetTypes();

    /**
     * @brief Executes all listeners for the given event type.
     * 
     * @param type The event type to fire.
     * @param data The data to send to all listeners.
     */
    static void Dispatch(const std::string& type, const sol::table& data);

    /// @brief Executes all listeners for event types relating to SDL.
    static void DoSDLEvents(const SDL_Event& event);

    /// @brief Removes all listeners in the list.
    static void CleanUp();

  protected:
    /// @brief The type of event being listened to.
    std::string type_;
    /// @brief Is the event listener active?
    bool active_;
    /// @brief The callback function to call when the event is fired.
    sol::function func_;
    /// @brief The list of event types, and their listeners.
    static ListenerMap listeners_;
  };
}

#endif // ! LISTENER_H
