/*
 * Copyright 2020
 * James Holtom
 * MIT License
 */

#ifndef FPS_MANAGER_H
#define FPS_MANAGER_H

#include "Timer.h"

namespace term_engine::timing {
  constexpr int FPS_FRAME_MULTIPLE = 5;

  void InitFPS();
  void Delay();
  void CalculateFPS();

  uint64_t GetFrameCount();
  void NextFrame();

  float GetAverageFPS();

  bool isUsingTargetFPS();
  int GetTargetFPS();
  void SetTargetFPS(const int& target);
  void DisableTargetFPS();

  void PrintFPS();

  extern Timer delay_timer_;
  extern Timer average_timer_;
}

#endif // ! FPS_MANAGER_H
