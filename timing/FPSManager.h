/*
 * Copyright 2020
 * James Holtom
 * MIT License
 */

#ifndef FPS_MANAGER_H
#define FPS_MANAGER_H

#include "Timer.h"

namespace term_engine::timing {
  void InitFPS();
  void Delay();
  void NextFrame();

  float GetAverageFPS();

  bool isUsingTargetFPS();
  int GetTargetFPS();
  void SetTargetFPS(const int& target);
  void DisableTargetFPS();

  extern Timer delay_timer_;
  extern Timer average_timer_;
}

#endif // ! FPS_MANAGER_H
