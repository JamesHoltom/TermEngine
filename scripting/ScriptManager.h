/*
 * Copyright 2020
 * James Holtom
 * MIT License
 */

#ifndef SCRIPT_MANAGER_H
#define SCRIPT_MANAGER_H

/*
  * Sol currently has an issue regarding deprecation error C4996.
  * https://github.com/ThePhD/sol2/issues/945
  *
  * After MSVC 16.6.0, there is an issue building Sol.
  */
#pragma warning(push)
#pragma warning(disable: 4996)

#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

#pragma warning(pop)

#include "Assert.h"

namespace term_engine::scripting {
  class ScriptManager {
  public:
    ScriptManager();
    ~ScriptManager();

  private:

    sol::state lua_state_;
  };
}


#endif
