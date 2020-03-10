/*
 * Copyright 2020
 * James Holtom
 * MIT License
 */

#ifndef UTIL_MOUSE_BINDING_H
#define UTIL_MOUSE_BINDING_H

#include <memory>
#include <string>

#include "Binding.h"
#include "../SDLIncludes.h"

namespace term_engine::utilities {
  struct Mouse {
    Uint8 button;
    Uint8 clicks;
    Sint32 mouse_x;
    Sint32 mouse_y;
    Sint32 wheel_x;
    Sint32 wheel_y;
  };

  class MouseBinding : public Binding {

  };
}

#endif // ! UTIL_MOUSE_BINDING_H
