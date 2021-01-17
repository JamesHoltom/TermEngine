/// @author James Holtom

#ifndef FPS_MANAGER_H
#define FPS_MANAGER_H

#include "Timer.h"

namespace term_engine::timing {
  /// Denotes how many frames to wait before updating the average frame rate.
  constexpr int FPS_FRAME_MULTIPLE = 5;

  /// The timer used to delay the program for the rest of the current frame.
  extern Timer delay_timer_;
  /// The timer used to calculate the average frame rate.
  extern Timer average_timer_;
  /// Should the frame rate be capped to a target rate?
  extern bool use_target_;
  /// The average frame rate while running the program.
  extern float average_fps_;
  /// The target frame rate the program can run up to, if it set.
  extern int target_fps_;
  /// The number of frames that have run since the program was started.
  extern uint64_t frame_count_;
  /// If a target frame rate is set, this is the amount of time between frames. E.g. A target of 10FPS means that _frame_duration_ is 0.1, or 100ms.
  extern uint64_t frame_duration_;

  /// Prepares the timers ready for calculating/capping the frame rate.
  void InitFPS();

  /// Tells the program to wait for however
  void Delay();

  /// Calculates the average frame rate.
  void CalculateFPS();

  /// Retrieves the total amount of frames that have passed since starting the program.
  /**
   * @returns How many frames have passed since the program started.
   */
  uint64_t GetFrameCount();

  /// Moves to the next frame of execution.
  void NextFrame();

  /// Retrieves the average frame rate.
  /**
   * @returns The average frame rate.
   */
  float GetAverageFPS();

  /// Is the frame rate being capped to a target rate?
  /**
   * @returns If the frame rate is being capped to a target rate.
   */
  bool isUsingTargetFPS();

  /// Retrieves the target frame rate.
  /**
   * @returns The target frame rate, or 0 if it is disabled.
   */
  int GetTargetFPS();

  /// Sets the target frame rate.
  /**
   * @param[in] target The target frame rate to set.
   */
  void SetTargetFPS(const int& target);

  /// Disables the target frame rate.
  void DisableTargetFPS();

  /// Logs the average and target frame rates.
  void PrintFPS();
}

#endif // ! FPS_MANAGER_H
