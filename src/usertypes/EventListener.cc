#include "EventListener.h"
#include "../scripting/ScriptingInterface.h"
#include "../utility/ImGuiUtils.h"
#include "../utility/SpdlogUtils.h"

namespace term_engine::usertypes {
  uint64_t EventListener::listener_next_id_ = 1;

  EventListener::EventListener(const std::string& type, const sol::protected_function callback) :
    Flaggable(),
    listener_id_(listener_next_id_++),
    type_(type),
    callback_(sol::make_reference<sol::protected_function>((*scripting::lua_state), callback)),
    times_fired_(0),
    is_active_(true)
  {
    utility::logger->debug("Created event listener with ID {} and type {}.", listener_id_, type);
  }

  EventListener::~EventListener()
  {
    callback_ = sol::nil;

    utility::logger->debug("Destroyed event listener with ID {} and type {}.", listener_id_, type_);
  }

  void EventListener::DoEvents(const events::Event& event)
  {
    if (is_active_ && event.type_ == type_)
    {
      Trigger(event.data_);
    }
  }

  uint64_t EventListener::GetListenerId() const
  {
    return listener_id_;
  }

  std::string EventListener::GetListenerType() const
  {
    return type_;
  }

  uint32_t EventListener::GetTimesFired() const
  {
    return times_fired_;
  }

  bool EventListener::IsActive() const
  {
    return is_active_;
  }

  void EventListener::SetActive(bool flag)
  {
    is_active_ = flag;
  }

  void EventListener::Trigger(const sol::table& data)
  {
    if (!callback_.valid())
    {
      return;
    }
    
    try {
      sol::protected_function_result result = callback_(this, data);
      ++times_fired_;

      if (!result.valid())
      {
        sol::error err = result;
        utility::logger->error("An error was thrown!\nProject: {}\n Error: {}", scripting::project_path.string(), err.what());
      }
    }
    catch (const std::exception& err) {
      utility::logger->error("A scripting error occurred!\nProject: {}\nError: {}", scripting::project_path.string(), err.what());
    }
  }

  void EventListener::UpdateDebugInfo() const
  {
    if (ImGui::TreeNode((void*)this, "EventListener #%li", listener_id_))
    {
      ImGui::Text("ID: %li", listener_id_);
      ImGui::Text("Active?: %s", is_active_ ? "Yes" : "No");
      ImGui::Text("Type: %s", type_.c_str());
      ImGui::Text("Times Fired: %u", times_fired_);

      ImGui::TreePop();
    }
  }

  EventListener* AddEventListener(const std::string& type, const sol::function callback)
  {
    listener_list.emplace_front(std::make_unique<EventListener>(type, callback));

    return static_cast<EventListener*>(listener_list.front().get());
  }

  EventListener* GetEventListenerById(uint64_t id)
  {
    for (auto& listener : listener_list)
    {
      if (listener->GetListenerId() == id)
      {
        return listener.get();
      }
    }

    return nullptr;
  }

  void RemoveEventListener(uint64_t id)
  {
    for (auto listener = listener_list.begin(); listener != listener_list.end(); listener++)
    {
      if ((*listener)->GetListenerId() == id) {
        (*listener)->FlagForRemoval();
        
        return;
      }
    };

    utility::logger->warn("Could not find event listener with ID {} to remove.", id);
  }

  void RemoveEventListener(EventListener* ptr)
  {
    for (auto listener = listener_list.begin(); listener != listener_list.end(); listener++)
    {
      if ((*listener).get() == ptr) {
        (*listener)->FlagForRemoval();
        
        return;
      }
    };

    utility::logger->warn("Could not find event listener to remove.");
  }

  void ClearAllEventListeners()
  {
    listener_list.clear();

    utility::logger->debug("Cleared all event listeners from the list.");
  }

  void ClearFlaggedEventListeners()
  {
    listener_list.remove_if([](const EventListenerPtr& listener)
    {
      return listener->FlaggedForRemoval();
    });
  }
}