/*
 * Copyright 2020
 * James Holtom
 * MIT License
 */

#ifndef MOD_TIMER_H
#define MOD_TIMER_H

#include <chrono>

using Milli = std::chrono::milliseconds;
using Clock = std::chrono::steady_clock;
using Duration = std::chrono::duration<int, Milli>;
using TimePoint = Clock::time_point;

namespace term_engine::modules {
  class Timer {
  public:
    Timer();

    void Start();
    void Stop();
    void Pause();
    void Resume();

    bool IsStarted() const;
    bool IsPaused() const;

    uint64_t GetDuration();
    uint64_t GetPauseDuration();
    uint64_t GetIntervalElapsed();

  private:
    TimePoint start_point_;
    TimePoint stop_point_;
    TimePoint pause_point_;
    TimePoint interval_point_;

    bool is_started_;
    bool is_paused_;
  };
}

#endif // !MOD_TIMER_H
