#include "TimedFunction.h"
#include "../../scripting/ScriptingInterface.h"
#include "../../utility/ImGuiUtils.h"
#include "../../utility/SpdlogUtils.h"

namespace term_engine::usertypes {
  TimedFunction::TimedFunction(uint64_t delay, bool started, bool repeat, const sol::function callback) :
    BaseObject(),
    accumulator_(0),
    delay_(delay),
    repeat_(repeat),
    times_fired_(0),
    callback_(sol::make_reference<sol::function>((*scripting::lua_state), callback))
  {
    is_active_ = started;
    
    utility::logger->debug("Created {} timed function with ID {} and delay of {}ms.", repeat ? "repeatable" : "non-repeatable", object_id_, delay_);
  }

  TimedFunction::~TimedFunction()
  {
    callback_ = sol::nil;

    utility::logger->debug("Removed timed function with ID {}.", object_id_);
  }

  void TimedFunction::Update(uint64_t timestep)
  {
    if (is_active_)
    {
      accumulator_ += timestep;

      if (accumulator_ >= delay_)
      {
        times_fired_++;

        try
        {
          sol::protected_function_result result = callback_(this, accumulator_);

          if (!result.valid())
          {
            sol::error err = result;
            utility::logger->error("An error was thrown!\nProject: {}\n Error: {}", scripting::project_path.string(), err.what());
          }
        }
        catch (const std::exception& err)
        {
          utility::logger->error("A scripting error occurred!\nProject: {}\nError: {}", scripting::project_path.string(), err.what());
        }

        if (repeat_)
        {
          accumulator_ = 0;
        }
        else
        {
          Stop();
        }
      }
    }
  }

  std::string TimedFunction::GetObjectType() const
  {
    return std::string(TIMED_FUNCTION_TYPE);
  }

  ObjectSortPriority TimedFunction::GetListPriority() const
  {
    return ObjectSortPriority::TIMED_FUNCTION;
  }

  uint64_t TimedFunction::GetDelay() const
  {
    return delay_;
  }

  bool TimedFunction::IsRepeatable() const
  {
    return repeat_;
  }

  uint32_t TimedFunction::GetTimesCalled() const
  {
    return times_fired_;
  }

  void TimedFunction::Start()
  {
    accumulator_ = 0;
    times_fired_ = 0;
    is_active_ = true;
  }

  void TimedFunction::Stop()
  {
    accumulator_ = 0;
    is_active_ = false;
  }

  void TimedFunction::SetRepeatable(bool flag)
  {
    repeat_ = flag;
  }

  void TimedFunction::UpdateDebugInfo() const
  {
    if (ImGui::TreeNode((void*)this, "%s #%li", GetObjectType().c_str(), object_id_))
    {
      ImGui::Text("ID: %lu", object_id_);
      ImGui::Text("Active?: %s", is_active_ ? "Yes" : "No");
      ImGui::Text("Repeat?: %s", repeat_ ? "Yes" : "No");
      ImGui::Text("Delay: %lu", delay_);
      ImGui::Text("Times Fired: %u", times_fired_);
      
      ImGui::Text("Progress:");
      ImGui::SameLine();

      char buf[32];
      sprintf(buf, "%li/%li", accumulator_, delay_);
      ImGui::ProgressBar((float)accumulator_ / (float)delay_, ImVec2(0.0f, 0.0f), buf);

      ImGui::TreePop();
    }
  }

  TimedFunction* AddTimedFunction(uint64_t delay, bool started, bool repeat, const sol::function callback)
  {
    int64_t set_delay = delay;

    if (delay < 0)
    {
      utility::logger->warn("Invalid delay of {}ms given. Defaulting to 0.", delay);

      set_delay = 0;
    }

    is_object_list_dirty = true;

    object_list.emplace_front(std::make_shared<TimedFunction>(set_delay, started, repeat, callback));

    return static_cast<TimedFunction*>(object_list.front().get());
  }

  TimedFunction* GetTimedFunctionById(uint64_t id)
  {
    for (auto& object : object_list)
    {
      if (object->GetObjectId() == id)
      {
        if (object->GetObjectType() == std::string(TIMED_FUNCTION_TYPE))
        {
          return static_cast<TimedFunction*>(object.get());
        }
      }
      else
      {
        utility::logger->warn("Object with ID {} is the name of a(n) {}.", id, object->GetObjectType());

        return nullptr;
      }
    }

    return nullptr;
  }
}