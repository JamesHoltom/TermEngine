/*
 * Copyright 2020
 * James Holtom
 * MIT License
 */

#ifndef UTIL_MOUSE_MANAGER_H
#define UTIL_MOUSE_MANAGER_H

#include "../types.h"
#include "../sdl_includes.h"

namespace term_engine::modules {
  struct Mouse {
    bool is_held;
    int held_frames;
  };

  class MouseManager {
  public:
    MouseManager();

    void HandleEvent(const SDL_Event& event);
    void UpdateFrames();

    bool GetMouseDown(const uint8_t& type) const;
    bool GetMousePress(const uint8_t& type) const;
    bool GetMouseRelease(const uint8_t& type) const;

    utilities::IntVector2D GetMousePosition() const;
    utilities::IntVector2D GetRelativeMousePosition() const;
    utilities::IntVector2D GetMouseScroll() const;

    int GetFramesHeld(const uint8_t& type) const;

  private:
    Mouse left_button_;
    Mouse middle_button_;
    Mouse right_button_;
    int position_x_, position_y_;
    int rel_position_x_, rel_position_y_;
    int scroll_x_, scroll_y_;
  };
}

#endif // ! UTIL_MOUSE_MANAGER_H
