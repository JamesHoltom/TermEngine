/// @author James Holtom

#ifndef TIMER_H
#define TIMER_H

#include <chrono>

namespace term_engine::timing {
  /// Used to measure time.
  using Clock = std::chrono::steady_clock;
  /// Represents a span of time, in seconds.
  using Duration = std::chrono::duration<float>;
  /// Represents a specific point in time.
  using TimePoint = Clock::time_point;

  /// Used to measure durations of time.
  class Timer {
  public:
    /// Constructs a _Timer_ object.
    Timer();

    /// Starts the timer.
    void Start();

    /// Stops the timer.
    void Stop();

    /// Pauses the timer, if it is running.
    void Pause();

    /// Resumes the timer, if it is paused.
    void Resume();

    /// Checks if the timer is running.
    /**
     * @returns Is the timer running?
     */
    bool IsStarted() const;

    /// Checks if the timer is paused.
    /**
     * @returns Is the timer paused?
     */
    bool IsPaused() const;

    /// Returns the duration of time since the timer was started.
    /**
     * This is excludes any time when the timer is paused.
     * 
     * @returns The duration of time since the timer was started.
     */
    uint64_t GetDuration();

    /// Returns the duration of time the timer was paused for.
    /**
     * @returns The duration of time the timer was paused for.
     */
    uint64_t GetPauseDuration();

    /// Returns the duration of time since the last interval, in milliseconds.
    /**
     * _interval_point_ is also set to now, so the next call to _GetIntervalElapsed_ gets the interval since this call.
     * 
     * @returns The duration of time since the timer was started, or when _GetIntervalElapsed_ was last called.
     */
    uint64_t GetIntervalElapsedMs();

    /// Returns the duration of time since the last interval, in seconds.
    /**
     * _interval_point_ is also set to now, so the next call to _GetIntervalElapsed_ gets the interval since this call.
     * 
     * @returns The duration of time since the timer was started, or when _GetIntervalElapsed_ was last called.
     */
    float GetIntervalElapsed();

  private:
    /// The point in time when the _Timer_ was started.
    Clock::time_point start_point_;
    /// The point in time when the _Timer_ was stopped.
    Clock::time_point stop_point_;
    /// The point in time when the _Timer_ was paused.
    Clock::time_point pause_point_;
    /// The point in time when an interval was triggered, i.e. when the timer started or when the interval was last checked.
    Clock::time_point interval_point_;
    /// Is the timer running?
    bool is_started_;
    /// Is the timer paused?
    bool is_paused_;
  };
}

#endif // ! TIMER_H
