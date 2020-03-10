/*
 * Copyright 2020
 * James Holtom
 * MIT License
 */

#ifndef UTIL_MOUSE_MANAGER_H
#define UTIL_MOUSE_MANAGER_H

namespace term_engine::modules {
  enum MouseButton { NONE, LEFT, MIDDLE, RIGHT };
  
  struct Mouse {
	  bool is_held;
    int held_frames;
  }
  
  class MouseManager {
  public:
    MouseManager();
    
    void UpdateFrames();
    void HandleEvent(const SDL_Event& event);
    
    bool GetMouseDown(const MouseButton& type);
    bool GetMousePress(const MouseButton& type);
    bool GetMouseRelease(const MouseButton& type);
    
  private:
    Mouse left_button_;
    Mouse middle_button_;
    Mouse right_button_;
  }
}

#endif // ! UTIL_MOUSE_MANAGER_H
