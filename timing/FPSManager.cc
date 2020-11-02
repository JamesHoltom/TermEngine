#include <chrono>
#include <thread>

#include "FPSManager.h"
#include "../utility/spdlogUtils.h"

namespace term_engine::timing {
  Timer delay_timer_;
  Timer average_timer_;

  bool use_target_;
  float average_fps_;
  int target_fps_;

  uint64_t frame_count_;
  uint64_t frame_duration_;

  void InitFPS() {
    delay_timer_.Start();
    average_timer_.Start();

    spdlog::debug("Started FPS.");
  }

  void Delay() {
    uint64_t time_taken = delay_timer_.GetDuration();
    std::chrono::milliseconds time_remaining = std::chrono::milliseconds(frame_duration_ - time_taken);

    if (time_remaining.count() > 0) {
      std::this_thread::sleep_for(time_remaining);
    }

    delay_timer_.Start();
  }

  void CalculateFPS() {
    if (frame_count_ % FPS_FRAME_MULTIPLE == 0 && frame_count_ > 0) {
      float time_taken = (float)average_timer_.GetIntervalElapsed() / 1000.0f;
      average_fps_ = (float)FPS_FRAME_MULTIPLE / time_taken;
    }
  }

  uint64_t GetFrameCount() {
    return frame_count_;
  }

  void NextFrame() {
    ++frame_count_;
  }

  float GetAverageFPS() {
    return average_fps_;
  }

  bool isUsingTargetFPS() {
    return use_target_;
  }

  int GetTargetFPS() {
    return target_fps_;
  }

  void SetTargetFPS(const int& target) {
    if (target > 0) {
      use_target_ = true;
      target_fps_ = target;
      frame_duration_ = std::chrono::milliseconds(1000 / target_fps_).count();
    }
    else {
      spdlog::warn("Attempted to set FPS to invalid value %i.\n", target);
    }
  }

  void DisableTargetFPS() {
    use_target_ = false;
    target_fps_ = 0;
    frame_duration_ = 0;
  }

  void PrintFPS() {
    if (use_target_) {
      spdlog::info("Target FPS: {}", target_fps_);
    }
    spdlog::info("Average FPS: {}", GetAverageFPS());
    spdlog::info("Frame count: {}", frame_count_);
  }
}
