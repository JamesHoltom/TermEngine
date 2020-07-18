#include "MouseManager.h"

namespace term_engine::modules {
  MouseManager::MouseManager() :
    left_button_(Mouse()),
    middle_button_(Mouse()),
    right_button_(Mouse()),
    position_x_(0),
    position_y_(0),
    rel_position_x_(0),
    rel_position_y_(0),
    scroll_x_(0),
    scroll_y_(0) {}

  void MouseManager::HandleEvent(const SDL_Event& event) {
    switch (event.type) {
    case SDL_MOUSEBUTTONDOWN:
    case SDL_MOUSEBUTTONUP:
      switch (event.button.button) {
      case SDL_BUTTON_LEFT:
        left_button_.is_held = (event.type == SDL_MOUSEBUTTONDOWN);

        break;
      case SDL_BUTTON_MIDDLE:
        middle_button_.is_held = (event.type == SDL_MOUSEBUTTONDOWN);

        break;
      case SDL_BUTTON_RIGHT:
        right_button_.is_held = (event.type == SDL_MOUSEBUTTONDOWN);

        break;
      }

      break;
    case SDL_MOUSEMOTION:
      position_x_ = event.motion.x;
      position_y_ = event.motion.y;
      rel_position_x_ = event.motion.xrel;
      rel_position_y_ = event.motion.yrel;

      break;
    case SDL_MOUSEWHEEL:
      scroll_x_ = event.wheel.x;
      scroll_y_ = event.wheel.y;
    }
  }

  void MouseManager::UpdateFrames() {
    if (left_button_.is_held) {
      left_button_.held_frames++;
    }
    else {
      left_button_.held_frames = 0;
    }

    if (middle_button_.is_held) {
      middle_button_.held_frames++;
    }
    else {
      middle_button_.held_frames = 0;
    }

    if (right_button_.is_held) {
      right_button_.held_frames++;
    }
    else {
      right_button_.held_frames = 0;
    }
  }

  bool MouseManager::GetMouseDown(const uint8_t& type) const {
    switch (type) {
    case SDL_BUTTON_LEFT:
      return left_button_.is_held;
    case SDL_BUTTON_MIDDLE:
      return middle_button_.is_held;
    case SDL_BUTTON_RIGHT:
      return right_button_.is_held;
    default:
      return false;
    }
  }

  bool MouseManager::GetMousePress(const uint8_t& type) const {
    switch (type) {
    case SDL_BUTTON_LEFT:
      return left_button_.is_held && left_button_.held_frames == 0;
    case SDL_BUTTON_MIDDLE:
      return middle_button_.is_held && middle_button_.held_frames == 0;
    case SDL_BUTTON_RIGHT:
      return right_button_.is_held && right_button_.held_frames == 0;
    default:
      return false;
    }
  }

  bool MouseManager::GetMouseRelease(const uint8_t& type) const {
    switch (type) {
    case SDL_BUTTON_LEFT:
      return !left_button_.is_held && left_button_.held_frames > 0;
    case SDL_BUTTON_MIDDLE:
      return !middle_button_.is_held && middle_button_.held_frames > 0;
    case SDL_BUTTON_RIGHT:
      return !right_button_.is_held && right_button_.held_frames > 0;
    default:
      return false;
    }
  }

  int MouseManager::GetFramesHeld(const uint8_t& type) const {
    switch (type) {
    case SDL_BUTTON_LEFT:
      return left_button_.held_frames;
    case SDL_BUTTON_MIDDLE:
      return middle_button_.held_frames;
    case SDL_BUTTON_RIGHT:
      return right_button_.held_frames;
    default:
      return -1;
    }
  }

  utilities::IntVector2D MouseManager::GetMousePosition() const {
    return std::make_tuple(position_x_, position_y_);
  }

  utilities::IntVector2D MouseManager::GetRelativeMousePosition() const {
    return std::make_tuple(rel_position_x_, rel_position_y_);
  }

  utilities::IntVector2D MouseManager::GetMouseScroll() const {
    return std::make_tuple(scroll_x_, scroll_y_);
  }
}
