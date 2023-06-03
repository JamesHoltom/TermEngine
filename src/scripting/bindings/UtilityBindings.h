/// @author James Holtom

#ifndef UTILITY_BINDINGS_H
#define UTILITY_BINDINGS_H

#include <cmath>
#include "../../events/InputManager.h"
#include "../../system/FileFunctions.h"
#include "../../system/FPSManager.h"
#include "../../usertypes/Character.h"
#include "../../usertypes/Window.h"
#include "../../usertypes/resources/Font.h"
#include "../../utility/ConversionUtils.h"
#include "../../utility/LogUtils.h"
#include "../../utility/SolUtils.h"

namespace term_engine::scripting::bindings {
  /**
   * @brief Binds any utility script functions to the Lua state.
   * 
   * @param[in] state The lua state to bind to.
   */
  void BindUtilitiesToState(sol::state& state)
  {
    state.create_named_table("characters",
      "NO_CHARACTER", sol::var(usertypes::NO_CHARACTER),
      "DEFAULT_FOREGROUND_COLOUR", sol::var(usertypes::DEFAULT_FOREGROUND_COLOUR),
      "DEFAULT_BACKGROUND_COLOUR", sol::var(usertypes::DEFAULT_BACKGROUND_COLOUR));
    
    state.create_named_table("fps",
      "print", &system::PrintFPS,
      "getAverage", &system::GetAverageFPS,
      "usingTarget", &system::IsUsingTargetFPS,
      "target", sol::overload(&system::GetTargetFPS, &system::SetTargetFPS),
      "getFrames", &system::GetFrameCount);

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

    state.set_function("getIndexFromPosition", sol::overload(
      sol::resolve<uint32_t(usertypes::GameObject*, const glm::ivec2&)>(&utility::GetIndexFromPosition),
      sol::resolve<uint32_t(usertypes::GameScene*, const glm::ivec2&)>(&utility::GetIndexFromPosition)));
    state.set_function("getPositionFromIndex", sol::overload(
      sol::resolve<glm::ivec2(usertypes::GameObject*, uint32_t)>(&utility::GetPositionFromIndex),
      sol::resolve<glm::ivec2(usertypes::GameScene*, uint32_t)>(&utility::GetPositionFromIndex)));
    state.set_function("getRowColFromPosition", sol::overload(
      sol::resolve<glm::ivec2(usertypes::GameObject*, const glm::ivec2&)>(&utility::GetRowColFromPosition),
      sol::resolve<glm::ivec2(usertypes::GameScene*, const glm::ivec2&)>(&utility::GetRowColFromPosition)));
    state.set_function("getPositionFromRowCol", sol::overload(
      sol::resolve<glm::ivec2(usertypes::GameObject*, const glm::ivec2&)>(&utility::GetPositionFromRowCol),
      sol::resolve<glm::ivec2(usertypes::GameScene*, const glm::ivec2&)>(&utility::GetPositionFromRowCol)));
    state.set_function("getRowColFromIndex", sol::overload(
      sol::resolve<glm::ivec2(usertypes::GameObject*, uint32_t)>(&utility::GetRowColFromIndex),
      sol::resolve<glm::ivec2(usertypes::GameScene*, uint32_t)>(&utility::GetRowColFromIndex)));
    state.set_function("getIndexFromRowCol", sol::overload(
      sol::resolve<uint32_t(usertypes::GameObject*, const glm::ivec2&)>(&utility::GetIndexFromRowCol),
      sol::resolve<uint32_t(usertypes::GameScene*, const glm::ivec2&)>(&utility::GetIndexFromRowCol)));

    state.create_named_table("window",
      "vsync", sol::overload(&usertypes::Window::IsVsyncEnabled, &usertypes::Window::SetVsync));
  }
}

#endif // ! UTILITY_BINDINGS_H
