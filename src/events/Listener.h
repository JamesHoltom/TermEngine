/// @author James Holtom

#ifndef LISTENER_H
#define LISTENER_H

#include <vector>
#include <string>
#include "../utility/SDLUtils.h"
#include "../utility/SolUtils.h"

namespace term_engine::events {
  struct Event;

  constexpr size_t MAX_EVENTS = 100;
  
  typedef std::vector<Event> EventQueue;

  struct Event {
    std::string type_;
    sol::table data_;

    Event(const std::string& type, const sol::table& data) :
      type_(type),
      data_(data)
    {}
  };

  extern EventQueue event_queue;

  void InitQueue();

  void CleanUpQueue();

  /// @brief Executes all listeners for event types relating to SDL.
  void DoSDLEvents(const SDL_Event& event);
}

#endif // ! LISTENER_H
