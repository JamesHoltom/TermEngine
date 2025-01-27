#include "InputManager.h"
#include "../utility/LogUtils.h"

namespace term_engine::events {
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

  void UpdateInputState()
  {
    current_mouse_state = SDL_GetMouseState(&mouse_position.x, &mouse_position.y);
    SDL_GetRelativeMouseState(&mouse_position_delta.x, &mouse_position_delta.y);
    current_key_state = SDL_GetKeyboardState(nullptr);
  }

  void UpdatePrevInputState()
  {
    previous_mouse_state = current_mouse_state;
    std::copy(current_key_state, current_key_state + key_state_size, previous_key_state);
  }

  bool MouseIsDown(int button)
  {
    return current_mouse_state & SDL_BUTTON(button);
  }

  bool MouseIsPressed(int button)
  {
    bool result = (current_mouse_state & SDL_BUTTON(button)) && !(previous_mouse_state & SDL_BUTTON(button));

    return result;
  }

  bool MouseIsReleased(int button)
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

  bool ScancodeIsDown(SDL_Scancode code)
  {
    if (code != SDL_SCANCODE_UNKNOWN)
    {
      return current_key_state[code];
    }
    else
    {
      utility::LogWarn("Unknown scancode '{}' passed to ScancodeIsDown.", (int)code);

      return false;
    }
  }

  bool KeyIsPressed(const std::string& key)
  {
    SDL_Scancode code = SDL_GetScancodeFromName(key.c_str());

    return ScancodeIsPressed(code);
  }

  bool ScancodeIsPressed(SDL_Scancode code)
  {
    if (code != SDL_SCANCODE_UNKNOWN)
    {
      return current_key_state[code] && !previous_key_state[code];
    }
    else
    {
      utility::LogWarn("Unknown scancode '{}' passed to ScancodeIsPressed.", (int)code);

      return false;
    }
  }

  bool KeyIsReleased(const std::string& key)
  {
    SDL_Scancode code = SDL_GetScancodeFromName(key.c_str());

    return ScancodeIsReleased(code);
  }

  bool ScancodeIsReleased(SDL_Scancode code)
  {
    if (code != SDL_SCANCODE_UNKNOWN)
    {
      return !current_key_state[code] && previous_key_state[code];
    }
    else
    {
      utility::LogWarn("Unknown scancode '{}' passed to ScancodeIsReleased.", (int)code);

      return false;
    }
  }

  bool IsTextInputModeActive()
  {
    return SDL_IsTextInputActive() == SDL_TRUE;
  }

  void StartTextInputMode()
  {
    SDL_StartTextInput();
  }

  void StopTextInputMode()
  {
    SDL_StopTextInput();
  }

  bool IsClipboardFilled()
  {
    return SDL_HasClipboardText() == SDL_TRUE;
  }

  std::string GetClipboard()
  {
    return std::string(SDL_GetClipboardText());
  }

  void SetClipboard(const std::string& text)
  {
    if (SDL_SetClipboardText(text.c_str()) == 0)
    {
      utility::LogDebug("Set clipboard to \"{}\"", text);
    }
    else
    {
      utility::LogError("Failed to set clipboard text! Error: {}", SDL_GetError());
    }
  }
}
