/// @author James Holtom

#ifndef LISTENER_H
#define LISTENER_H

#include <vector>
#include <string>
#include "../usertypes/GameScene.h"
#include "../utility/SDLUtils.h"
#include "../utility/SolUtils.h"

namespace term_engine::events {
  struct Event;

  /// @brief The maximum number of SDL_Events that can be polled at a time.
  constexpr uint64_t MAX_EVENTS = 100;
  
  /// @brief Used to store a list of events.
  typedef std::vector<Event> EventList;

  /// @brief Represents an event fired by SDL, and emitted to the EventListeners.
  struct Event {
    /**
     * @brief Constructs the event with the given parameters.
     * 
     * @param[in] type        The type of event.
     * @param[in] game_scene  The game scene this listener is bound to.
     * @param[in] data        The data to pass to EventListeners.
     */
    Event(const std::string& type, usertypes::GameScene* game_scene, const sol::table& data) :
      type_(type),
      game_scene_(game_scene),
      data_(data)
    {}

    /// @brief The type of event.
    std::string type_;
    /// @brief The game scene this listener is bound to.
    usertypes::GameScene* game_scene_;
    /// @brief The data to pass to EventListeners.
    sol::table data_;
  };

  /// @brief List of events from SDL, that get emitted to the EventListeners.
  extern EventList event_list;

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
