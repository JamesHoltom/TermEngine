#include "InputManager.h"

namespace term_engine::events {
  void Init() {
    current_key_state = SDL_GetKeyboardState(&key_state_size);
    old_key_state = new Uint8[key_state_size];
  }

  void CleanUp() {
    delete[] old_key_state;
    old_key_state = nullptr;

    current_key_state = nullptr;
  }

  void UpdateEvents() {
    current_mouse_state = SDL_GetMouseState(&mouse_position.x, &mouse_position.y);
    SDL_GetRelativeMouseState(&mouse_position_delta.x, &mouse_position_delta.y);
    current_key_state = SDL_GetKeyboardState(nullptr);
  }

  void UpdatePrevEvents() {
    old_mouse_state = current_mouse_state;
    std::copy(current_key_state, current_key_state + key_state_size, old_key_state);
  }

  bool MouseIsDown(const int& button) {
    return current_mouse_state & SDL_BUTTON(button);
  }

  bool MouseIsPressed(const int& button) {
    bool result = (current_mouse_state & SDL_BUTTON(button)) && !(old_mouse_state & SDL_BUTTON(button));

    return result;
  }

  bool MouseIsReleased(const int& button) {
    bool result = !(current_mouse_state & SDL_BUTTON(button)) && (old_mouse_state & SDL_BUTTON(button));

    return result;
  }

  bool KeyIsDown(const std::string& key) {
    return current_key_state[SDL_GetScancodeFromName(key.c_str())];
  }

  bool KeyIsPressed(const std::string& key) {
    SDL_Scancode code = SDL_GetScancodeFromName(key.c_str());
    bool result = current_key_state[code] && !old_key_state[code];

    return result;
  }

  bool KeyIsReleased(const std::string& key) {
    SDL_Scancode code = SDL_GetScancodeFromName(key.c_str());
    bool result = !current_key_state[code] && old_key_state[code];

    return result;
  }

  const Uint8* current_key_state = nullptr;
  Uint8* old_key_state = nullptr;
  int key_state_size;

  Uint32 current_mouse_state;
  Uint32 old_mouse_state;
  glm::ivec2 mouse_position;
  glm::ivec2 mouse_position_delta;
}
