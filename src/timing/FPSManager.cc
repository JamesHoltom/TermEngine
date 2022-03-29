#include <chrono>
#include <thread>

#include "FPSManager.h"
#include "../logging/Logger.h"

namespace term_engine::timing {
  Timer delay_timer_;
  Timer average_timer_;
  float average_fps_;
  int target_fps_;
  uint64_t frame_count_;
  uint64_t frame_duration_;

  void InitFPS()
  {
    delay_timer_.Start();
    average_timer_.Start();

    logging::logger->debug("Started FPS.");
  }

  void Delay()
  {
    uint64_t time_taken = delay_timer_.GetDuration();
    std::chrono::milliseconds time_remaining = std::chrono::milliseconds(frame_duration_ - time_taken);

    if (time_remaining.count() > 0) {
      std::this_thread::sleep_for(time_remaining);
    }

    delay_timer_.Start();
  }

  void CalculateFPS()
  {
    if (frame_count_ % FPS_FRAME_MULTIPLE == 0 && frame_count_ > 0) {
      float time_taken = average_timer_.GetIntervalElapsed();
      average_fps_ = (float)FPS_FRAME_MULTIPLE / time_taken;
    }
  }

  uint64_t GetFrameCount()
  {
    return frame_count_;
  }

  void NextFrame()
  {
    ++frame_count_;
  }

  float GetAverageFPS()
  {
    return average_fps_;
  }

  bool IsUsingTargetFPS()
  {
    return target_fps_ > 0;
  }

  int GetTargetFPS()
  {
    return target_fps_;
  }

  void SetTargetFPS(const int& target)
  {
    if (target > 0) {
      target_fps_ = target;
      frame_duration_ = std::chrono::milliseconds(1000 / target_fps_).count();
    }
    else {
      target_fps_ = 0;
      frame_duration_ = 0;
    }
  }

  void PrintFPS()
  {
    if (target_fps_ > 0) {
      logging::logger->info("Target FPS: {}", target_fps_);
    }
    logging::logger->info("Average FPS: {}", GetAverageFPS());
    logging::logger->info("Frame count: {}", frame_count_);
  }
}
