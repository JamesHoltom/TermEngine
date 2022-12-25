/// @author James Holtom

#ifndef TIMER_H
#define TIMER_H

#include <chrono>

namespace timing {
  /// @brief Used to measure time.
  using Clock = std::chrono::steady_clock;
  /// @brief Represents a span of time, in seconds.
  using Duration = std::chrono::duration<float>;
  /// @brief Represents a specific point in time.
  using TimePoint = Clock::time_point;

  /// @brief Used to measure durations of time.
  class Timer {
  public:
    /// @brief Constructs a _Timer_ object.
    Timer();

    /// @brief Starts the timer.
    void Start();

    /// @brief Stops the timer.
    void Stop();

    /// @brief Pauses the timer, if it is running.
    void Pause();

    /// @brief Resumes the timer, if it is paused.
    void Resume();

    /**
     * @brief Checks if the timer is running.
     * 
     * @returns Is the timer running?
     */
    bool IsStarted() const;

    /**
     * @brief Checks if the timer is paused.
     * 
     * @returns Is the timer paused?
     */
    bool IsPaused() const;

    /**
     * @brief Returns the duration of time since the timer was started.
     * @details This excludes any time when the timer is paused.
     * 
     * @returns The duration of time since the timer was started.
     */
    uint64_t GetDuration();

    /**
     * @brief Returns the duration of time the timer was paused for.
     * 
     * @returns The duration of time the timer was paused for.
     */
    uint64_t GetPauseDuration();

    /**
     * @brief Returns the duration of time since the last interval, in milliseconds.
     * @details _interval_point_ is also set to now, so the next call to _GetIntervalElapsed_ gets the interval since this call.
     * 
     * @returns The duration of time since the timer was started, or when _GetIntervalElapsed_ was last called.
     */
    uint64_t GetIntervalElapsedMs();

    /**
     * @brief Returns the duration of time since the last interval, in seconds.
     * @details _interval_point_ is also set to now, so the next call to _GetIntervalElapsed_ gets the interval since this call.
     * 
     * @returns The duration of time since the timer was started, or when _GetIntervalElapsed_ was last called.
     */
    float GetIntervalElapsed();

  private:
    /// @brief The point in time when the _Timer_ was started.
    Clock::time_point start_point_;
    /// @brief The point in time when the _Timer_ was stopped.
    Clock::time_point stop_point_;
    /// @brief The point in time when the _Timer_ was paused.
    Clock::time_point pause_point_;
    /// @brief The point in time when an interval was triggered, i.e. when the timer started or when the interval was last checked.
    Clock::time_point interval_point_;
    /// @brief Is the timer running?
    bool is_started_;
    /// @brief Is the timer paused?
    bool is_paused_;
  };
}

#endif // ! TIMER_H
