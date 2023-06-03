#include "Timer.h"

namespace term_engine::usertypes {
  Timer::Timer() :
    start_point_(TimePoint()),
    stop_point_(TimePoint()),
    pause_point_(TimePoint()),
    interval_point_(TimePoint()),
    is_started_(false),
    is_paused_(false)
  {}

  void Timer::Start()
  {
    is_started_ = true;
    is_paused_ = false;

    start_point_ = Clock::now();
    stop_point_ = TimePoint();
    pause_point_ = TimePoint();
    interval_point_ = start_point_;
  }

  void Timer::Stop()
  {
    is_started_ = false;
    is_paused_ = false;

    stop_point_ = Clock::now();
    pause_point_ = TimePoint();
    interval_point_ = TimePoint();
  }

  void Timer::Pause()
  {
    if (is_started_ && !is_paused_) {
      is_paused_ = true;

      pause_point_ = Clock::now();
    }
  }

  void Timer::Resume()
  {
    if (is_started_ && is_paused_) {
      is_paused_ = false;

      start_point_ += Clock::now() - pause_point_;
    }
  }

  bool Timer::IsStarted() const
  {
    return is_started_;
  }

  bool Timer::IsPaused() const
  {
    return is_paused_;
  }

  uint64_t Timer::GetDuration()
  {
    uint64_t duration;

    if (is_started_) {
      if (is_paused_) {
        duration = std::chrono::duration_cast<std::chrono::milliseconds>(pause_point_ - start_point_).count();
      }
      else {
        duration = std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - start_point_).count();
      }
    }
    else {
      duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop_point_ - start_point_).count();
    }

    return duration;
  }

  uint64_t Timer::GetPauseDuration()
  {
    uint64_t duration;

    if (is_started_ && is_paused_) {
      duration = std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - pause_point_).count();
    }
    else {
      duration = 0;
    }

    return duration;
  }

  uint64_t Timer::GetIntervalElapsed()
  {
    uint64_t elapsed;
    Clock::time_point now = Clock::now();

    if (is_started_ && !is_paused_) {
      elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - interval_point_).count();
      interval_point_ = now;
    }
    else {
      elapsed = 0;
    }

    return elapsed;
  }
}