/*
 * Copyright 2020
 * James Holtom
 * MIT License
 */

#ifndef SCRIPT_MANAGER_H
#define SCRIPT_MANAGER_H

#include "../utility/SolUtils.h"

namespace term_engine::scripting {
  class ScriptManager {
  public:
    ScriptManager();
    ~ScriptManager();

    void RunInit();

    void RunTest();

  private:

    sol::state lua_state_;
  };
}


#endif // ! SCRIPT_MANAGER_H
