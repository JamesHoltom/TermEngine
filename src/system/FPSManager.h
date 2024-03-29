/// @author James Holtom

#ifndef FPS_MANAGER_H
#define FPS_MANAGER_H

#include "../usertypes/Timer.h"

namespace term_engine::system {
  /// @brief Denotes how many frames to wait before updating the average frame rate.
  constexpr uint32_t FPS_FRAME_MULTIPLE = 5;
  /// @brief Represents the default FPS when starting the program.
  constexpr uint32_t DEFAULT_FPS = 60;

  /// @brief The timer used to delay the program for the rest of the current frame.
  inline usertypes::Timer delay_timer_;
  /// @brief The timer used to calculate the average frame rate.
  inline usertypes::Timer average_timer_;
  /// @brief The average frame rate while running the program.
  inline float average_fps_ = 0.0f;
  /// @brief The target frame rate the program can run up to, if it set.
  inline uint32_t target_fps_ = 0;
  /// @brief The number of frames that have run since the program was started.
  inline uint64_t frame_count_ = 0;
  /// @brief If a target frame rate is set, this is the amount of time between frames. E.g. A target of 10FPS means that _frame_duration_ is 0.1, or 100ms.
  inline uint64_t frame_duration_ = 0;

  /// @brief Prepares the timers ready for calculating/capping the frame rate.
  void InitFPS();

  /// @brief Tells the program to wait until the next frame starts (if a target framerate is set).
  uint64_t Delay();

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
  uint32_t GetTargetFPS();

  /**
   * @brief Sets the target frame rate. Setting this to 0 will disable the target framerate.
   * 
   * @param[in] target The target frame rate to set.
   */
  void SetTargetFPS(uint32_t target);

  /// @brief Logs the average and target frame rates.
  void PrintFPS();
}

#endif // ! FPS_MANAGER_H
