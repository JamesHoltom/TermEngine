#include "TimedFunction.h"
#include "../scripting/ScriptingInterface.h"
#include "../utility/SpdlogUtils.h"

namespace term_engine::objects {
  TimedFunction::TimedFunction(const int& delay, const bool& repeat, const sol::function callback) :
    BaseObject(),
    delay_(delay),
    repeat_(repeat),
    times_repeated_(0),
    callback_(sol::make_reference<sol::function>((*scripting::lua_state), callback))
  {
    timer_.Start();

    utility::logger->debug("Created {} timed function with ID {} and delay of {}ms.", repeat ? "repeatable" : "non-repeatable", object_id_, delay_);
  }

  TimedFunction::~TimedFunction()
  {
    callback_ = sol::nil;

    utility::logger->debug("Removed timed function with ID {}.", object_id_);
  }

  void TimedFunction::Update()
  {
    if (is_active_)
    {
      if (!timer_.IsStarted())
      {
        timer_.Start();
        times_repeated_ = 0;
      }

      uint64_t duration = timer_.GetDuration();

      if (duration >= delay_)
      {
        times_repeated_++;

        try
        {
          sol::protected_function_result result = callback_(this, duration);

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

        if (repeat_)
        {
          timer_.Start();
        }
        else
        {
          timer_.Stop();
          is_active_ = false;
        }
      }
    }
    else
    {
      if (timer_.IsStarted())
      {
        timer_.Stop();
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

  int TimedFunction::GetDelay() const
  {
    return delay_;
  }

  bool TimedFunction::IsRepeatable() const
  {
    return repeat_;
  }

  int TimedFunction::GetTimesRepeated() const
  {
    return times_repeated_;
  }

  TimedFunctionProxy::TimedFunctionProxy(const ObjectPtr& object) :
    BaseProxy(object)
  {}

  TimedFunctionProxy::~TimedFunctionProxy()
  {}

  TimedFunctionProxy AddTimedFunction(const int& delay, const bool& repeat, const sol::function callback)
  {
    int set_delay = delay;

    if (delay < 0)
    {
      utility::logger->warn("Invalid delay of {}ms given. Defaulting to 0.", delay);

      set_delay = 0;
    }

    is_object_list_dirty_ = true;

    return TimedFunctionProxy(object_list_.emplace_front(std::make_shared<TimedFunction>(set_delay, repeat, callback)));
  }

  void ClearAllTimedFunctions()
  {
    object_list_.remove_if([](const ObjectPtr& object) { return object->GetObjectType() == std::string(TIMED_FUNCTION_TYPE); });

    utility::logger->debug("Cleared all timed functions from the list.");
  }
}