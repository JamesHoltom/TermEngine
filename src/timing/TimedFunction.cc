#include "TimedFunction.h"
#include "../scripting/ScriptingInterface.h"
#include "../logging/Logger.h"

namespace term_engine::timing {
  TimedFunction::TimedFunction(const int& delay, const bool& repeat, const sol::function callback) :
    timed_id_(TimedFunction::timed_next_id_++),
    active_(true),
    delay_(delay),
    repeat_(repeat),
    times_repeated_(0),
    callback_(sol::make_reference<sol::function>((*scripting::lua_state), callback))
  {
    timer_.Start();

    logging::logger->debug("Created timed function.");
  }

  bool TimedFunction::IsActive() const
  {
    return active_;
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

  void TimedFunction::SetActive(const bool& flag)
  {
    if (flag && !active_)
    {
      times_repeated_ = 0;
      timer_.Start();
    }
    else if (!flag && active_)
    {
      timer_.Stop();
    }

    active_ = flag;
  }

  void TimedFunction::Update()
  {
    for (TimedFunctionPtr& it : function_list_)
    {
      uint64_t duration = it->timer_.GetDuration();

      if (it->active_ && duration >= it->delay_)
      {
        it->times_repeated_++;

        try {
          sol::protected_function_result result = it->callback_(it, duration);

          if (!result.valid())
          {
            sol::error err = result;
            logging::logger->error("An error was thrown!\nFile: {}\n Error: {}", scripting::lua_file, err.what());
          }
        }
        catch (const std::exception& err) {
          logging::logger->error("A scripting error occurred!\nFile: {}\nError: {}", scripting::lua_file, err.what());
        }

        if (it->repeat_)
        {
          it->timer_.Start();
        }
        else
        {
          it->active_ = false;
        }
      }
    }
  }

  TimedFunctionPtr& TimedFunction::Add(const int& delay, const bool& repeat, const sol::function callback)
  {
    logging::logger->debug("Added {}repeatable timed function with delay of {}ms", (repeat ? "" : "non-"), delay);

    return function_list_.emplace_back(std::make_shared<TimedFunction>(delay, repeat, callback));
  }

  TimedFunctionPtr& TimedFunction::AddSelf(sol::object self, const int& delay, const bool& repeat, const sol::function callback)
  {
    return Add(delay, repeat, callback);
  }

  void TimedFunction::Remove(TimedFunctionPtr& timed_function)
  {
    TimedFunctionList::iterator it = std::find(function_list_.begin(), function_list_.end(), timed_function);

    if (it != function_list_.end())
    {
      function_list_.erase(it);

      logging::logger->debug("Removed timed function. {} refs found.", timed_function.use_count());
    }
    else
    {
      logging::logger->warn("Couldn't find timed function in list!");
    }

    timed_function.reset();
  }

  int TimedFunction::Count()
  {
    return function_list_.size();
  }

  void TimedFunction::CleanUp()
  {
    for (TimedFunctionPtr& timed_function : function_list_)
    {
      Remove(timed_function);
    }
  }

  int TimedFunction::timed_next_id_ = 0;
  TimedFunctionList TimedFunction::function_list_;
}