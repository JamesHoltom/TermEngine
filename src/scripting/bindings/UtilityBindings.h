/// @author James Holtom

#ifndef UTILITY_BINDINGS_H
#define UTILITY_BINDINGS_H

#include <cmath>
#include "../IndexFunctions.h"
#include "../../events/InputManager.h"
#include "../../rendering/Character.h"
#include "../../rendering/FontAtlas.h"
#include "../../rendering/Window.h"
#include "../../system/AudioFunctions.h"
#include "../../system/FileFunctions.h"
#include "../../timing/FPSManager.h"
#include "../../utility/DebugUtils.h"
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
    state.create_named_table("audio",
      "trigger", &system::PlaySound);

    state.create_named_table("characters",
      "NO_CHARACTER", sol::var(rendering::NO_CHARACTER),
      "DEFAULT_FOREGROUND_COLOUR", sol::var(rendering::DEFAULT_FOREGROUND_COLOUR),
      "DEFAULT_BACKGROUND_COLOUR", sol::var(rendering::DEFAULT_BACKGROUND_COLOUR));
    
    state.create_named_table("debug_info",
      "get", &utility::GetDebugInfo,
      "getCount", &utility::GetDebugInfoCount,
      "getLineCount", &utility::GetDebugInfoLines);
    
    state.create_named_table("fps",
      "print", &timing::PrintFPS,
      "getAverage", &timing::GetAverageFPS,
      "usingTarget", &timing::IsUsingTargetFPS,
      "target", sol::overload(&timing::GetTargetFPS, &timing::SetTargetFPS),
      "getFrames", &timing::GetFrameCount);

    state.create_named_table("font",
      "load", &rendering::GetFontAtlas,
      "getDefault", &rendering::GetDefaultFont,
      "getDefaultSize", &rendering::GetDefaultFontSize,
      "tabSize", sol::overload(&rendering::GetTabSize, &rendering::SetTabSize));

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
      sol::resolve<uint32_t(objects::GameObject*, const glm::ivec2&)>(&scripting::GetIndexFromPosition),
      sol::resolve<uint32_t(objects::GameScene*, const glm::ivec2&)>(&scripting::GetIndexFromPosition)));
    state.set_function("getPositionFromIndex", sol::overload(
      sol::resolve<glm::ivec2(objects::GameObject*, uint32_t)>(&scripting::GetPositionFromIndex),
      sol::resolve<glm::ivec2(objects::GameScene*, uint32_t)>(&scripting::GetPositionFromIndex)));
    state.set_function("getRowColFromPosition", sol::overload(
      sol::resolve<glm::ivec2(objects::GameObject*, const glm::ivec2&)>(&utility::GetRowColFromPosition),
      sol::resolve<glm::ivec2(objects::GameScene*, const glm::ivec2&)>(&utility::GetRowColFromPosition)));
    state.set_function("getPositionFromRowCol", sol::overload(
      sol::resolve<glm::ivec2(objects::GameObject*, const glm::ivec2&)>(&utility::GetPositionFromRowCol),
      sol::resolve<glm::ivec2(objects::GameScene*, const glm::ivec2&)>(&utility::GetPositionFromRowCol)));
    state.set_function("getRowColFromIndex", sol::overload(
      sol::resolve<glm::ivec2(objects::GameObject*, uint32_t)>(&scripting::GetRowColFromIndex),
      sol::resolve<glm::ivec2(objects::GameScene*, uint32_t)>(&scripting::GetRowColFromIndex)));
    state.set_function("getIndexFromRowCol", sol::overload(
      sol::resolve<uint32_t(objects::GameObject*, const glm::ivec2&)>(&utility::GetIndexFromRowCol),
      sol::resolve<uint32_t(objects::GameScene*, const glm::ivec2&)>(&utility::GetIndexFromRowCol)));

    state.create_named_table("window",
      "vsync", sol::overload(&rendering::GameWindow::IsVsyncEnabled, &rendering::GameWindow::SetVsync));
  }
}

#endif // ! UTILITY_BINDINGS_H
