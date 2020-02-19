/*
 * Copyright 2020
 * James Holtom
 * MIT License
 */

#ifndef MOD_FPS_MANAGER_H
#define MOD_FPS_MANAGER_H

#include "Timer.h"

namespace term_engine::modules {
  class FPSManager {
  public:
    FPSManager();
    FPSManager(const int& target);

    void Delay();
    void NextFrame();

    float GetAverageFPS();

    bool isUsingTargetFPS() const;
    int GetTargetFPS() const;
    void SetTargetFPS(const int& target);
    void DisableTargetFPS();

  private:
    Timer delay_timer_;
    Timer average_timer_;

    bool use_target_;
    float average_fps_;
    int target_fps_;

    uint64_t frame_count_;
    uint64_t frame_duration_;
  };
}

#endif
