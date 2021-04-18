/// @author James Holtom

#ifndef SCRIPTING_INTERFACE_H
#define SCRIPTING_INTERFACE_H

#include <string>

#include "../objects/BaseObject.h"
#include "../utility/SolUtils.h"

namespace term_engine::scripting {
  /// The location of the "No Program" script to run, only if a project was not loaded.
  constexpr char DEFAULT_SCRIPT_PATH[] = "resources/scripts/init.lua";

  /// Creates the Lua functions and usertypes that interface with TermEngine.
  void InitInterface();

  /// Selects the project script to execute, or the "No Program" script if one is not set.
  void InitScript();

  /// Runs garbage collection on the Lua state.
  void CleanUp();

  /// Runs a Lua function, and returns the result of execution.
  /**
   * @param[in] function_name The name of the Lua function to run.
   * @returns The result of running this function, and any errors that have occurred from execution.
   */
  sol::protected_function_result Run(const std::string& function_name);

  /// Loads the contents of a Lua file into the state, allowing subsequent scripts to use the contents.
  /**
   * @param[in] filename The file name/path of the Lua script to load.
   */
  void Load(const std::string& filename);

  /// Runs the "Init" Lua function, which is used to execute game code on startup.
  /**
   * @returns A boolean value, defined by the script as if initialisation was successful.
   */
  bool OnInit();

  /// Runs the "Loop" Lua function, which is used to execute game code every frame.
  void OnLoop();

  /// Runs the "Quit" Lua function, which runs game code before closing TermEngine.
  /**
   * @returns A boolean value, indicating if TermEngine can close. This can be used by scripters to ask the player before closing, or to run clean-up code.
   */
  bool OnQuit();

  /// The Lua state that stores all of the loaded script functions, variables, etc.
  extern sol::state lua_state;

  /// The filepath of the currently loaded project.
  extern std::string lua_file;
}

#endif // ! SCRIPTING_INTERFACE_H
