/// @author James Holtom

#ifndef LISTENER_H
#define LISTENER_H

#include <vector>
#include <string>
#include "../usertypes/GameWindow.h"
#include "../utility/SDLUtils.h"
#include "../utility/SolUtils.h"

namespace term_engine::events {
  struct Event;

  /// @brief Used to store a list of events.
  typedef std::vector<Event> EventList;

  /// @brief The maximum number of SDL_Events that can be polled at a time.
  constexpr uint64_t MAX_EVENTS = 100;
  
  /// @brief List of events from SDL, that get emitted to the EventListeners.
  inline EventList event_list;

  /// @brief Represents an event fired by SDL, and emitted to the EventListeners.
  struct Event {
    /**
     * @brief Constructs the event with the given parameters.
     * 
     * @param[in] type        The type of event.
     * @param[in] game_window The game window this listener is bound to.
     * @param[in] data        The data to pass to EventListeners.
     */
    Event(const std::string& type, usertypes::GameWindow* game_window, const sol::table& data) :
      type_(type),
      game_window_(game_window),
      data_(data)
    {}

    /// @brief The type of event.
    std::string type_;
    /// @brief The game window this listener is bound to.
    usertypes::GameWindow* game_window_;
    /// @brief The data to pass to EventListeners.
    sol::table data_;
  };

  /// @brief Prepares the event list.
  void InitList();

  /// @brief Clears the event list.
  void CleanUpList();

  /**
   * @brief Executes all listeners for event types relating to SDL.
   * 
   * @param[in] event The event to check and broadcast.
   */
  void DoSDLEvents(const SDL_Event& event);
}

#endif // ! LISTENER_H
