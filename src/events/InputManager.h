/// @author James Holtom

#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <array>
#include <string>
#include <glm/glm.hpp>

#include "../utility/SDLUtils.h"

namespace term_engine::events {
  /// Human-readable names for the mouse buttons when logging.
  constexpr std::array<char[8], 4> mouse_button_names = {{
    { "Unknown" },
    { "Left" },
    { "Middle" },
    { "Right" }
  }};

  /// A pointer to SDL's internal array of key states, for the current frame.
  extern const Uint8* current_key_state;
  /// A pointer to a copy of SDL's internal array of key states, for the previous frame.
  extern Uint8* previous_key_state;
  /// Holds the size of SDL's internal array of key states.
  extern int key_state_size;
  /// Holds a bitfield representing the mouse button states, for the current frame.
  extern Uint32 current_mouse_state;
  /// Holds a bitfield representing the mouse button states, for the previous frame.
  extern Uint32 previous_mouse_state;
  /// Represents the location of the mouse, in relation to the window.
  extern glm::ivec2 mouse_position;
  /// Represents how far the mouse has moved within the window.
  extern glm::ivec2 mouse_position_delta;

  /// Prepares the keyboard states for use.
  void Init();

  /// Removes the keyboard states from memory.
  void CleanUp();

  /// Updates the keyboard and mouse states for the current frame.
  void UpdateEvents();

  /// Stores the current frame's keyboard/mouse states, ready for the next frame's update.
  void UpdatePrevEvents();

  /// Checks if a mouse button is being pressed.
  /**
   * @param[in] button Which mouse button to check. This is normally `SDL_BUTTON_LEFT`, `SDL_BUTTON_MIDDLE` or `SDL_BUTTON_RIGHT`.
   * @returns If the mouse button is being pressed.
   */
  bool MouseIsDown(const int& button);

  /// Checks if a mouse button has just been pressed, i.e. it was pressed this frame, but not in the previous frame.
  /** 
   * @param[in] button Which mouse button to check. This is normally `SDL_BUTTON_LEFT`, `SDL_BUTTON_MIDDLE` or `SDL_BUTTON_RIGHT`.
   * @returns If the mouse button has just been pressed this frame.
   */
  bool MouseIsPressed(const int& button);

  /// Checks if a mouse button has just been released, i.e. it was pressed in the previous frame, and released this frame.
  /** 
   * @param[in] button Which mouse button to check. This is normally `SDL_BUTTON_LEFT`, `SDL_BUTTON_MIDDLE` or `SDL_BUTTON_RIGHT`.
   * @returns If the mouse button has just been released this frame.
   */
  bool MouseIsReleased(const int& button);

  /// Checks if a key is being pressed.
  /**
   * @param[in] key The key to check. This corresponds with a *Key Name* in SDL2.
   * @see [SDL key names](https://wiki.libsdl.org/SDL_Scancode)
   * @returns If the key is being pressed.
   */
  bool KeyIsDown(const std::string& key);

  /// Checks if a key has just been pressed, i.e. it was pressed this frame, but not in the previous frame.
  /**
   * @param[in] key The key to check. This corresponds with a *Key Name* in SDL2.
   * @see [SDL key names](https://wiki.libsdl.org/SDL_Scancode)
   * @returns If the key has just been pressed this frame.
   */
  bool KeyIsPressed(const std::string& key);

  /// Checks if a key has just been released, i.e. it was pressed in the previous frame, and released this frame.
  /**
   * @param[in] key The key to check. This corresponds with a *Key Name* in SDL2.
   * @see [SDL key names](https://wiki.libsdl.org/SDL_Scancode)
   * @returns If the key has just been released this frame.
   */
  bool KeyIsReleased(const std::string& key);
}

#endif // ! INPUT_MANAGER_H
