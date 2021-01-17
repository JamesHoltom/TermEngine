/// @author James Holtom

#ifndef SCRIPTING_INTERFACE_H
#define SCRIPTING_INTERFACE_H

#include <string>

#include "../utility/SolUtils.h"

namespace term_engine::scripting {
  void InitInterface();
  void InitScript();
  void CleanUp();

  sol::protected_function_result Run(const std::string& function_name);
  void Load(const std::string& filename);

  bool OnInit();
  void OnLoop();
  bool OnQuit();

  extern sol::state lua_state;
  extern std::string lua_file;
}

#endif // ! SCRIPTING_INTERFACE_H
