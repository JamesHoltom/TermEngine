#include <chrono>
#include <thread>

#include "FPSManager.h"

namespace term_engine::modules {
  constexpr int FPS_FRAME_MULTIPLE = 5;

  FPSManager::FPSManager() :
    use_target_(false),
    target_fps_(0),
    average_fps_(0.0f),
    frame_count_(0),
    frame_duration_(0) {
    delay_timer_.Start();
    average_timer_.Start();
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

  void FPSManager::Delay() {
    uint64_t time_taken = delay_timer_.GetDuration();
    std::chrono::milliseconds time_remaining = std::chrono::milliseconds(frame_duration_ - time_taken);

    if (time_remaining.count() > 0) {
      std::this_thread::sleep_for(time_remaining);
    }

    delay_timer_.Start();
  }

  void FPSManager::NextFrame() {
    ++frame_count_;
  }

  float FPSManager::GetAverageFPS() {
    if (frame_count_ % FPS_FRAME_MULTIPLE == 0 && frame_count_ > 0) {
      float time_taken = average_timer_.GetIntervalElapsed() / 1000.0f;
      average_fps_ = (float)FPS_FRAME_MULTIPLE / time_taken;
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
