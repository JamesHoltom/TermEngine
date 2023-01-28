/// @author James Holtom

#ifndef UTILITY_BINDINGS_H
#define UTILITY_BINDINGS_H

#include <cmath>
#include "../../events/InputManager.h"
#include "../../rendering/Character.h"
#include "../../rendering/Window.h"
#include "../../system/FileFunctions.h"
#include "../../timing/FPSManager.h"
#include "../../utility/IndexUtils.h"
#include "../../utility/SolUtils.h"

namespace term_engine::scripting::bindings {
  /**
   * @brief Binds any utility script functions to the Lua state.
   * 
   * @param[in] state The lua state to bind to.
   */
  void BindUtilitiesToState(sol::state& state)
  {
    state.create_named_table(
      "characters",
      "NO_CHARACTER", sol::var(rendering::NO_CHARACTER),
      "DEFAULT_FOREGROUND_COLOUR", sol::var(rendering::DEFAULT_FOREGROUND_COLOUR),
      "DEFAULT_BACKGROUND_COLOUR", sol::var(rendering::DEFAULT_BACKGROUND_COLOUR));
    
    state.create_named_table("fps",
      "print", &timing::PrintFPS,
      "getAverage", &timing::GetAverageFPS,
      "usingTarget", &timing::IsUsingTargetFPS,
      "target", sol::overload(&timing::GetTargetFPS, &timing::SetTargetFPS),
      "getFrames", &timing::GetFrameCount);

    state.create_named_table("fs",
      "read", &system::ReadFile,
      "write", &system::WriteFile,
      "exists", &system::FileExists,
      "getFiles", &system::GetFileList,
      "getFolders", &system::GetFolderList);

    state.create_named_table("keyboard",
      "isDown", &events::KeyIsDown,
      "isPressed", &events::KeyIsPressed,
      "isReleased", &events::KeyIsReleased);

    state.create_named_table("mouse",
      "LEFT", sol::var(SDL_BUTTON_LEFT),
      "MIDDLE", sol::var(SDL_BUTTON_MIDDLE),
      "RIGHT", sol::var(SDL_BUTTON_RIGHT),
      "isDown", &events::MouseIsDown,
      "isPressed", &events::MouseIsPressed,
      "isReleased", &events::MouseIsReleased,
      "getPosition", &events::GetMousePosition,
      "getMovement", &events::GetMouseMovement);

    state.set_function("round", lroundf);
    
    /* TODO: Fix these, taking into account the Lua-style index.
    state.set_function("getIndexFromPosition", sol::overload(
      sol::resolve<int(objects::GameScenePtr, glm::ivec2, glm::ivec2)>(utility::GetIndexFromPosition),
      sol::resolve<int(rendering::FontAtlas, glm::ivec2, glm::ivec2)>(utility::GetIndexFromPosition)));
    state.set_function("getRowColFromPosition", utility::GetRowColFromPosition);
    state.set_function("getPositionFromIndex", utility::GetPositionFromIndex);
    state.set_function("getRowColFromIndex", utility::GetRowColFromIndex);
    state.set_function("getIndexFromRowCol", utility::GetIndexFromRowCol);
    state.set_function("getPositionFromRowCol", utility::GetPositionFromRowCol);
    */

    state.create_named_table("window",
      "vsync", sol::overload(&rendering::Window::IsVsyncEnabled, &rendering::Window::SetVsync));
  }
}

#endif // ! UTILITY_BINDINGS_H
