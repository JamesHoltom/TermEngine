/*
 * Copyright 2020
 * James Holtom
 * MIT License
 */

#ifndef TIMER_H
#define TIMER_H

#include <chrono>

using Clock = std::chrono::steady_clock;
using Duration = std::chrono::duration<int, std::chrono::milliseconds>;
using TimePoint = Clock::time_point;

namespace term_engine::timing {
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
    Clock::time_point start_point_;
    Clock::time_point stop_point_;
    Clock::time_point pause_point_;
    Clock::time_point interval_point_;

    bool is_started_;
    bool is_paused_;
  };
}

#endif // ! TIMER_H
