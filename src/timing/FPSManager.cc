#include <chrono>
#include <thread>
#include "FPSManager.h"
#include "../utility/SpdlogUtils.h"

namespace term_engine::timing {
  Timer delay_timer_;
  Timer average_timer_;
  float average_fps_;
  uint32_t target_fps_;
  uint64_t frame_count_;
  uint64_t frame_duration_;

  void InitFPS()
  {
    delay_timer_.Start();
    average_timer_.Start();

    utility::logger->debug("Started FPS.");
  }

  uint64_t Delay()
  {
    uint64_t time_taken = delay_timer_.GetDuration();
    int64_t remaining = frame_duration_ - time_taken;
    
    if (remaining > 0) {
      std::this_thread::sleep_for(std::chrono::milliseconds(remaining));
    }

    delay_timer_.Start();

    return time_taken;
  }

  void CalculateFPS()
  {
    if (frame_count_ % FPS_FRAME_MULTIPLE == 0 && frame_count_ > 0) {
      float time_taken = (float)average_timer_.GetIntervalElapsed() / 1000.0f;
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

  uint32_t GetTargetFPS()
  {
    return target_fps_;
  }

  void SetTargetFPS(uint32_t target)
  {
    if (target > 0) {
      target_fps_ = target;
      frame_duration_ = 1000.0f / target_fps_;
    }
    else {
      target_fps_ = 0;
      frame_duration_ = 0;
    }
  }

  void PrintFPS()
  {
    if (target_fps_ > 0) {
      utility::logger->info("Target FPS: {}", target_fps_);
    }
    
    utility::logger->info("Average FPS: {}", GetAverageFPS());
    utility::logger->info("Frame count: {}", frame_count_);
  }
}
