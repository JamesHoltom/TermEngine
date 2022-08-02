/// @author James Holtom

#ifndef UTILITY_BINDINGS_H
#define UTILITY_BINDINGS_H

#include <random>
#include <string>
#include "../../resources/Window.h"
#include "../../timing/FPSManager.h"
#include "../../utility/SolUtils.h"

namespace term_engine::scripting::bindings {
  /// Binds any utility script functions to the Lua state.
  /**
   * @param[in] state The lua state to bind to.
   */
  void BindUtilitiesToState(sol::state& state)
  {
    state.create_named_table("fps",
      "print", &timing::PrintFPS,
      "average", &timing::GetAverageFPS,
      "usingTarget", &timing::IsUsingTargetFPS,
      "getTarget", &timing::GetTargetFPS,
      "setTarget", &timing::SetTargetFPS,
      "frames", &timing::GetFrameCount);

    state.create_named_table("random",
      "get", sol::overload(
        [&]() -> float { return (float)rand() / RAND_MAX; },
        [&](const float& range) -> float { return ((float)rand() / RAND_MAX) * range; },
        [&](const float& range, const float& offset) -> float { return (((float)rand() / RAND_MAX) * range) + offset; }
      ));

    state.create_named_table("wireframe",
      "enable", &system::EnableWireframe,
      "disable", &system::DisableWireframe);
  }
}

#endif // ! UTILITY_BINDINGS_H
