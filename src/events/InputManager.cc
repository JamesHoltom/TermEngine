#include "InputManager.h"
#include "../logging/Logger.h"

namespace term_engine::events {
  const Uint8* current_key_state = nullptr;
  Uint8* previous_key_state = nullptr;
  int key_state_size;
  Uint32 current_mouse_state;
  Uint32 previous_mouse_state;
  glm::ivec2 mouse_position;
  glm::ivec2 mouse_position_delta;

  void Init()
  {
    // Setting a parameter in SDL_GetKeyboardState will return the size of SDL's internal array, which will be required for initialising the previous frame's pointer.
    current_key_state = SDL_GetKeyboardState(&key_state_size);
    previous_key_state = new Uint8[key_state_size];
  }

  void CleanUp()
  {
    delete[] previous_key_state;
    previous_key_state = nullptr;

    // The current frame's pointer refers to data that SDL manages, and does not need to be deleted here.
    current_key_state = nullptr;
  }

  void UpdateEvents()
  {
    current_mouse_state = SDL_GetMouseState(&mouse_position.x, &mouse_position.y);
    SDL_GetRelativeMouseState(&mouse_position_delta.x, &mouse_position_delta.y);
    current_key_state = SDL_GetKeyboardState(nullptr);
  }

  void UpdatePrevEvents()
  {
    previous_mouse_state = current_mouse_state;
    std::copy(current_key_state, current_key_state + key_state_size, previous_key_state);
  }

  bool MouseIsDown(const int& button)
  {
    return current_mouse_state & SDL_BUTTON(button);
  }

  bool MouseIsPressed(const int& button)
  {
    bool result = (current_mouse_state & SDL_BUTTON(button)) && !(previous_mouse_state & SDL_BUTTON(button));

    return result;
  }

  bool MouseIsReleased(const int& button)
  {
    bool result = !(current_mouse_state & SDL_BUTTON(button)) && (previous_mouse_state & SDL_BUTTON(button));

    return result;
  }

  glm::ivec2 GetMousePosition()
  {
    return mouse_position;
  }

  glm::ivec2 GetMouseMovement()
  {
    return mouse_position_delta;
  }

  bool KeyIsDown(const std::string& key)
  {
    SDL_Scancode code = SDL_GetScancodeFromName(key.c_str());

    return ScancodeIsDown(code);
  }

  bool ScancodeIsDown(const SDL_Scancode& code)
  {
    if (code != SDL_SCANCODE_UNKNOWN)
    {
      return current_key_state[code];
    }
    else
    {
      logging::logger->warn("Unknown scancode '{}' passed to ScancodeIsDown.", code);

      return false;
    }
  }

  bool KeyIsPressed(const std::string& key)
  {
    SDL_Scancode code = SDL_GetScancodeFromName(key.c_str());

    return ScancodeIsPressed(code);
  }

  bool ScancodeIsPressed(const SDL_Scancode& code)
  {
    if (code != SDL_SCANCODE_UNKNOWN)
    {
      return current_key_state[code] && !previous_key_state[code];
    }
    else
    {
      logging::logger->warn("Unknown scancode '{}' passed to ScancodeIsPressed.", code);

      return false;
    }
  }

  bool KeyIsReleased(const std::string& key)
  {
    SDL_Scancode code = SDL_GetScancodeFromName(key.c_str());

    return ScancodeIsReleased(code);
  }

  bool ScancodeIsReleased(const SDL_Scancode& code)
  {
    if (code != SDL_SCANCODE_UNKNOWN)
    {
      return !current_key_state[code] && previous_key_state[code];
    }
    else
    {
      logging::logger->warn("Unknown scancode '{}' passed to ScancodeIsReleased.", code);

      return false;
    }
  }
}
