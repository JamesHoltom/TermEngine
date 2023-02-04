#include "EventListener.h"
#include "../events/Listener.h"
#include "../scripting/ScriptingInterface.h"
#include "../utility/SpdlogUtils.h"

namespace term_engine::objects {
  EventListener::EventListener(const GameSceneWeakPtr& game_scene, const std::string& type, const sol::function callback) :
    BaseObject(),
    game_scene_(game_scene),
    type_(type),
    callback_(sol::make_reference<sol::function>((*scripting::lua_state), callback))
  {
    utility::logger->debug("Created event listener with ID {} and type {}.", object_id_, type);
  }

  EventListener::~EventListener()
  {
    callback_ = sol::nil;
    utility::logger->debug("Destroyed event listener with ID {} and type {}.", object_id_, type_);
  }

  void EventListener::Update()
  {
    for (const events::Event& event : events::event_queue)
    {
      if (event.type_ == type_)
      {
        try
        {
          sol::protected_function_result result = callback_(this, event.data_);

          if (!result.valid())
          {
            sol::error err = result;
            utility::logger->error("An error was thrown!\nFile: {}\n Error: {}", scripting::lua_file, err.what());
          }
        }
        catch (const std::exception& err)
        {
          utility::logger->error("A scripting error occurred!\nFile: {}\nError: {}", scripting::lua_file, err.what());
        }
      }
    }
  }

  std::string EventListener::GetObjectType() const
  {
    return std::string(EVENT_LISTENER_TYPE);
  }

  ObjectSortPriority EventListener::GetListPriority() const
  {
    return ObjectSortPriority::EVENT_LISTENER;
  }

  GameSceneWeakPtr EventListener::GetGameScene() const
  {
    return game_scene_;
  }

  std::string EventListener::GetListenerType() const
  {
    return type_;
  }

  void EventListener::Trigger(const sol::table& data)
  {
    try {
      sol::protected_function_result result = callback_(this, data);

      if (!result.valid())
      {
        sol::error err = result;
        utility::logger->error("An error was thrown!\nFile: {}\n Error: {}", scripting::lua_file, err.what());
      }
    }
    catch (const std::exception& err) {
      utility::logger->error("A scripting error occurred!\nFile: {}\nError: {}", scripting::lua_file, err.what());
    }
  }

  EventListenerProxy::EventListenerProxy(const ObjectPtr& object) :
    BaseProxy(object)
  {}

  EventListenerProxy::~EventListenerProxy()
  {}

  EventListenerProxy AddEventListenerToScene(const std::string& type, const std::string& name, const sol::function callback)
  {
    is_object_list_dirty_ = true;

    GameScenePtr game_scene = std::dynamic_pointer_cast<GameScene>(GetGameSceneByName(name).lock());
    
    return EventListenerProxy(object_list_.emplace_front(std::make_shared<EventListener>(game_scene, type, callback)));
  }

  EventListenerProxy AddEventListener(const std::string& type, const sol::function callback)
  {
    return AddEventListenerToScene(type, "default", callback);
  }

  void ClearAllEventListeners()
  {
    object_list_.remove_if([](const ObjectPtr& object) { return object->GetObjectType() == std::string(EVENT_LISTENER_TYPE); });

    utility::logger->debug("Cleared all event listeners from the list.");
  }

  void ClearEventListenersByGameScene(const std::string& name)
  {
    GameScenePtr game_scene = std::dynamic_pointer_cast<GameScene>(GetGameSceneByName(name).lock());

    object_list_.remove_if([&game_scene](const ObjectPtr& object) { return object->GetObjectType() == std::string(EVENT_LISTENER_TYPE) && std::dynamic_pointer_cast<EventListener>(object)->GetGameScene().lock() == game_scene; });

    utility::logger->debug("Cleared all event listeners for game scene \"{}\" from the list.", name);
  }
}