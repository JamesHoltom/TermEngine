/// @author James Holtom

#ifndef FPS_MANAGER_H
#define FPS_MANAGER_H

#include "Timer.h"

namespace timing {
  /// @brief Denotes how many frames to wait before updating the average frame rate.
  constexpr int FPS_FRAME_MULTIPLE = 5;
  /// @brief Represents the default FPS when starting the program.
  constexpr int DEFAULT_FPS = 60;

  /// @brief The timer used to delay the program for the rest of the current frame.
  extern Timer delay_timer_;
  /// @brief The timer used to calculate the average frame rate.
  extern Timer average_timer_;
  /// @brief The average frame rate while running the program.
  extern float average_fps_;
  /// @brief The target frame rate the program can run up to, if it set.
  extern int target_fps_;
  /// @brief The number of frames that have run since the program was started.
  extern uint64_t frame_count_;
  /// @brief If a target frame rate is set, this is the amount of time between frames. E.g. A target of 10FPS means that _frame_duration_ is 0.1, or 100ms.
  extern uint64_t frame_duration_;

  /// @brief Prepares the timers ready for calculating/capping the frame rate.
  void InitFPS();

  /// @brief Tells the program to wait for however
  void Delay();

  /// @brief Calculates the average frame rate.
  void CalculateFPS();

  /**
   * @brief Retrieves the total amount of frames that have passed since starting the program.
   * 
   * @returns How many frames have passed since the program started.
   */
  uint64_t GetFrameCount();

  /// @brief Moves to the next frame of execution.
  void NextFrame();

  /**
   * @brief Retrieves the average frame rate.
   * 
   * @returns The average frame rate.
   */
  float GetAverageFPS();

  /**
   * @brief Is the frame rate being capped to a target rate?
   * 
   * @returns If the frame rate is being capped to a target rate.
   */
  bool IsUsingTargetFPS();

  /**
   * @brief Retrieves the target frame rate.
   * 
   * @returns The target frame rate, or 0 if it is disabled.
   */
  int GetTargetFPS();

  /**
   * @brief Sets the target frame rate. Setting this to 0 will disable the target framerate.
   * 
   * @param[in] target The target frame rate to set.
   */
  void SetTargetFPS(const int& target);

  /// @brief Logs the average and target frame rates.
  void PrintFPS();
}

#endif // ! FPS_MANAGER_H
