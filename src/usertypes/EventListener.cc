#include "EventListener.h"
#include "../scripting/ScriptingInterface.h"
#include "../utility/ImGuiUtils.h"
#include "../utility/SpdlogUtils.h"

namespace term_engine::usertypes {
  uint64_t EventListener::listener_next_id_ = 1;

  EventListener::EventListener(GameScene* game_scene, const std::string& type, const sol::function callback) :
    listener_id_(listener_next_id_++),
    game_scene_(game_scene),
    type_(type),
    callback_(sol::make_reference<sol::function>((*scripting::lua_state), callback)),
    times_fired_(0),
    is_active_(true),
    marked_for_removal_(false)
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
      if (game_scene_ == nullptr || (game_scene_ != nullptr && event.game_scene_ == game_scene_))
      {
        Trigger(event.data_);
      }
    }
  }

  uint64_t EventListener::GetListenerId() const
  {
    return listener_id_;
  }

  GameScene* EventListener::GetGameScene() const
  {
    return game_scene_;
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

  bool EventListener::FlaggedForRemoval() const
  {
    return marked_for_removal_;
  }

  void EventListener::FlagForRemoval()
  {
    marked_for_removal_ = true;
  }

  void EventListener::UnflagForRemoval()
  {
    marked_for_removal_ = false;
  }

  void EventListener::Trigger(const sol::table& data)
  {
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
      ImGui::Text("Game Scene: %s", game_scene_ != nullptr ? game_scene_->GetName().c_str() : "<Not set>");

      ImGui::TreePop();
    }
  }

  EventListener* AddEventListenerToScene(const std::string& type, const sol::function callback, const std::string& name)
  {
    GameScene* game_scene = GetGameSceneByName(name);
    
    if (game_scene == nullptr)
    {
      utility::logger->warn("Cannot add event listener for non-existent game scene \"{}\"!", name);

      return nullptr;
    }
    else
    {
      listener_list.emplace_front(std::make_unique<EventListener>(game_scene, type, callback));

      return static_cast<EventListener*>(listener_list.front().get());
    }
  }

  EventListener* AddEventListener(const std::string& type, const sol::function callback)
  {
    listener_list.emplace_front(std::make_unique<EventListener>(nullptr, type, callback));

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

  void ClearEventListenersByGameScene(const std::string& name)
  {
    GameScene* game_scene = GetGameSceneByName(name);

    listener_list.remove_if([&game_scene](const EventListenerPtr& listener)
    {
      return listener->GetGameScene() == game_scene;
    });

    utility::logger->debug("Cleared all event listeners for game scene \"{}\" from the list.", name);
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