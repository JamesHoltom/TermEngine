/*
 * Copyright 2020
 * James Holtom
 * MIT License
 */

#ifndef SCRIPTING_INTERFACE_H
#define SCRIPTING_INTERFACE_H

#include <string>

#include "../utility/SolUtils.h"

namespace term_engine::scripting {
  void InitInterface();
  void Boot();
  void Run(const std::string& script_name);

  extern sol::state lua_state;
}

#endif // ! SCRIPTING_INTERFACE_H
