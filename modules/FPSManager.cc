#include <chrono>
#include <thread>

#include "FPSManager.h"

namespace term_engine::modules {
  FPSManager::FPSManager() :
    use_target_(false),
    target_fps_(0),
    average_fps_(0.0f),
    frame_count_(0),
    frame_duration_(0) {

  }
  FPSManager::FPSManager(const int& target) :
    use_target_(true),
    target_fps_(target),
    average_fps_(0.0f),
    frame_count_(0),
    frame_duration_(0)
  {
    SetTargetFPS(target);

    delay_timer_.Start();
    average_timer_.Start();
  }

  void FPSManager::DelayUntilInterval() {
    uint64_t time_taken = delay_timer_.GetIntervalElapsed();
    std::chrono::duration<uint64_t, std::milli> time_remaining = std::chrono::milliseconds(frame_duration_ - time_taken);

    ++frame_count_;

    std::this_thread::sleep_for(time_remaining);
  }

  float FPSManager::GetAverageFPS() {
    if (frame_count_ % 10 == 0 && frame_count_ > 0) {
      uint64_t time_taken = average_timer_.GetIntervalElapsed();
      average_fps_ = 10.0f / time_taken;
    }

    return average_fps_;
  }

  bool FPSManager::isUsingTargetFPS() const {
    return use_target_;
  }

  int FPSManager::GetTargetFPS() const {
    return target_fps_;
  }

  void FPSManager::SetTargetFPS(const int& target) {
    if (target > 0) {
      use_target_ = true;
      target_fps_ = target;
      frame_duration_ = std::chrono::milliseconds(1000 / target_fps_).count();
    }
    else {
      printf("Attempted to set FPS to invalid value %i.\n", target);
    }
  }

  void FPSManager::DisableTargetFPS() {
    use_target_ = false;
    target_fps_ = 0;
    frame_duration_ = 0;
  }
}
