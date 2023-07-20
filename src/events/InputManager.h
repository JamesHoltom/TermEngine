/// @author James Holtom

#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <array>
#include <string>
#include <glm/glm.hpp>
#include "../utility/SDLUtils.h"

namespace term_engine::events {
  /// @brief Human-readable names for the mouse buttons when logging.
  constexpr std::array<char[8], 4> mouse_button_names = {{
    { "Unknown" },
    { "Left" },
    { "Middle" },
    { "Right" }
  }};

  /// @brief A pointer to SDL's internal array of key states, for the current frame.
  inline const Uint8* current_key_state = nullptr;
  /// @brief A pointer to a copy of SDL's internal array of key states, for the previous frame.
  inline Uint8* previous_key_state = nullptr;
  /// @brief Holds the size of SDL's internal array of key states.
  inline int key_state_size = 0;
  /// @brief Holds a bitfield representing the mouse button states, for the current frame.
  inline Uint32 current_mouse_state = 0;
  /// @brief Holds a bitfield representing the mouse button states, for the previous frame.
  inline Uint32 previous_mouse_state = 0;
  /// @brief Represents the location of the mouse, in relation to the window.
  inline glm::ivec2 mouse_position;
  /// @brief Represents how far the mouse has moved within the window.
  inline glm::ivec2 mouse_position_delta;

  /// @brief Prepares the keyboard states for use.
  void Init();

  /// @brief Removes the keyboard states from memory.
  void CleanUp();

  /// @brief Updates the keyboard and mouse states for the current frame.
  void UpdateInputState();

  /// @brief Stores the current frame's keyboard/mouse states, ready for the next frame's update.
  void UpdatePrevInputState();

  /**
   * @brief Checks if a mouse button is being pressed.
   * 
   * @param[in] button Which mouse button to check. This is normally `SDL_BUTTON_LEFT`, `SDL_BUTTON_MIDDLE` or `SDL_BUTTON_RIGHT`.
   * @returns If the mouse button is being pressed.
   */
  bool MouseIsDown(int button);

  /** 
   * @brief Checks if a mouse button has just been pressed, i.e. it was pressed this frame, but not in the previous frame.
   * 
   * @param[in] button Which mouse button to check. This is normally `SDL_BUTTON_LEFT`, `SDL_BUTTON_MIDDLE` or `SDL_BUTTON_RIGHT`.
   * @returns If the mouse button has just been pressed this frame.
   */
  bool MouseIsPressed(int button);

  /** 
   * @brief Checks if a mouse button has just been released, i.e. it was pressed in the previous frame, and released this frame.
   * 
   * @param[in] button Which mouse button to check. This is normally `SDL_BUTTON_LEFT`, `SDL_BUTTON_MIDDLE` or `SDL_BUTTON_RIGHT`.
   * @returns If the mouse button has just been released this frame.
   */
  bool MouseIsReleased(int button);

  /**
   * @brief Returns the position of the mouse on the screen.
   * 
   * @returns The mouse position.
   */
  glm::ivec2 GetMousePosition();

  /**
   * @brief Returns the amount of mouse movement this frame.
   * 
   * @returns The amount of mouse movement.
   */
  glm::ivec2 GetMouseMovement();

  /**
   * @brief Checks if a key is being pressed.
   * 
   * @param[in] key The key to check. This corresponds with a *Key Name* in SDL2.
   * @see [SDL key names](https://wiki.libsdl.org/SDL_Scancode)
   * @returns If the key is being pressed.
   */
  bool KeyIsDown(const std::string& key);

  /**
   * @brief Checks if a key is being pressed.
   * 
   * @param[in] code The key to check. This corresponds with a *Scancode* in SDL2.
   * @see [SDL scancodes](https://wiki.libsdl.org/SDL_Scancode)
   * @returns If the key is being pressed.
   */
  bool ScancodeIsDown(SDL_Scancode code);

  /**
   * @brief Checks if a key has just been pressed, i.e. it was pressed this frame, but not in the previous frame.
   * 
   * @param[in] key The key to check. This corresponds with a *Key Name* in SDL2.
   * @see [SDL key names](https://wiki.libsdl.org/SDL_Scancode)
   * @returns If the key has just been pressed this frame.
   */
  bool KeyIsPressed(const std::string& key);

  /**
   * @brief Checks if a key has just been pressed, i.e. it was pressed this frame, but not in the previous frame.
   * 
   * @param[in] code The key to check. This corresponds with a *Scancode* in SDL2.
   * @see [SDL scancodes](https://wiki.libsdl.org/SDL_Scancode)
   * @returns If the key has just been pressed this frame.
   */
  bool ScancodeIsPressed(SDL_Scancode code);

  /**
   * @brief Checks if a key has just been released, i.e. it was pressed in the previous frame, and released this frame.
   * 
   * @param[in] key The key to check. This corresponds with a *Key Name* in SDL2.
   * @see [SDL key names](https://wiki.libsdl.org/SDL_Scancode)
   * @returns If the key has just been released this frame.
   */
  bool KeyIsReleased(const std::string& key);

  /**
   * @brief Checks if a key has just been released, i.e. it was pressed in the previous frame, and released this frame.
   * 
   * @param[in] code The key to check. This corresponds with a *Scancode* in SDL2.
   * @see [SDL scancodes](https://wiki.libsdl.org/SDL_Scancode)
   * @returns If the key has just been released this frame.
   */
  bool ScancodeIsReleased(SDL_Scancode code);

  /**
   * @brief Returns if SDL's "Text Input" mode is enabled.
   * @details SDL uses an API for typing characters that require multiple key presses.
   * 
   * @see https://wiki.libsdl.org/SDL2/Tutorials-TextInput
   * @returns If "Text Input" mode is enabled.
   */
  bool IsTextInputModeActive();

  /// @brief Enables SDL's "Text Input" mode.
  void StartTextInputMode();

  /// @brief Disables SDL's "Text Input" mode.
  void StopTextInputMode();

  /**
   * @brief Returns if the clipboard contains text.
   * 
   * @returns If the clipboard is filled.
   */
  bool IsClipboardFilled();

  /**
   * @brief Returns the contents of the clipboard.
   * 
   * @returns The clipboard contents.
   */
  std::string GetClipboard();

  /**
   * @brief Sets the contents of the clipboard.
   * 
   * @param[in] text The text to set.
   */
  void SetClipboard(const std::string& text);
}

#endif // ! INPUT_MANAGER_H
