/*
 * Copyright 2020
 * James Holtom
 * MIT License
 */

#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <array>
#include <string>
#include <glm/glm.hpp>

#include "../utility/SDLUtils.h"

namespace term_engine::events {
  constexpr std::array<char[8], 4> mouse_button_names = {{
    { "Unknown" },
    { "Left" },
    { "Middle" },
    { "Right" }
  }};

  void Init();
  void CleanUp();

  void UpdateEvents();
  void UpdatePrevEvents();

  bool MouseIsDown(const int& button);
  bool MouseIsPressed(const int& button);
  bool MouseIsReleased(const int& button);

  bool KeyIsDown(const std::string& key);
  bool KeyIsPressed(const std::string& key);
  bool KeyIsReleased(const std::string& key);

  extern const Uint8* current_key_state;
  extern Uint8* old_key_state;
  extern int key_state_size;

  extern Uint32 current_mouse_state;
  extern Uint32 old_mouse_state;
  extern glm::ivec2 mouse_position;
  extern glm::ivec2 mouse_position_delta;
}

#endif
