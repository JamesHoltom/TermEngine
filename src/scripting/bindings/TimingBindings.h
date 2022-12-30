/// @author James Holtom

#ifndef TIMING_BINDINGS_H
#define TIMING_BINDINGS_H

#include "../../timing/FPSManager.h"
#include "../../timing/Timer.h"
#include "../../timing/TimedFunction.h"
#include "../../utility/SolUtils.h"

namespace term_engine::scripting::bindings {
  /**
   * @brief Binds the timing-related script functions to the Lua state.
   * 
   * @param[in] state The lua state to bind to.
   */
  void BindTimingToState(sol::state& state)
  {
    state.create_named_table("fps",
      "print", &timing::PrintFPS,
      "getAverage", &timing::GetAverageFPS,
      "usingTarget", &timing::IsUsingTargetFPS,
      "getTarget", &timing::GetTargetFPS,
      "setTarget", &timing::SetTargetFPS,
      "getFrames", &timing::GetFrameCount);

    state.new_usertype<timing::Timer>(
      "Timer",
      sol::meta_function::construct, sol::constructors<void()>(),
      sol::call_constructor, sol::constructors<void()>(),
      "start", &timing::Timer::Start,
      "stop", &timing::Timer::Stop,
      "pause", &timing::Timer::Pause,
      "resume", &timing::Timer::Resume,
      "intervalMs", &timing::Timer::GetIntervalElapsedMs,
      "intervalSecs", &timing::Timer::GetIntervalElapsed,
      "started", sol::readonly_property(&timing::Timer::IsStarted),
      "paused", sol::readonly_property(&timing::Timer::IsPaused),
      "duration", sol::readonly_property(&timing::Timer::GetDuration),
      "paused_duration", sol::readonly_property(&timing::Timer::GetPauseDuration));

    state.new_usertype<timing::TimedFunction>(
      "TimedFunction",
      sol::meta_function::construct, sol::factories(&timing::TimedFunction::Add, &timing::TimedFunction::AddSelf),
      sol::call_constructor, sol::factories(&timing::TimedFunction::Add),
      sol::meta_function::garbage_collect, sol::destructor(&timing::TimedFunction::Remove),
      "active", sol::property(&timing::TimedFunction::IsActive, &timing::TimedFunction::SetActive),
      "delay", sol::readonly_property(&timing::TimedFunction::GetDelay),
      "repeat", sol::readonly_property(&timing::TimedFunction::IsRepeatable),
      "timesRepeated", sol::readonly_property(&timing::TimedFunction::GetTimesRepeated),
      "release", &timing::TimedFunction::Remove);
  }
}

#endif // ! TIMING_BINDINGS_H