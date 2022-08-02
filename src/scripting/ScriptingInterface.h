/// @author James Holtom

#ifndef SCRIPTING_INTERFACE_H
#define SCRIPTING_INTERFACE_H

#include <string>

#include "../utility/SolUtils.h"

namespace term_engine::scripting {
  /// The location of the "No Program" script to run, only if a project was not loaded.
  constexpr char DEFAULT_SCRIPT_PATH[] = "noprogram.lua";
  /// The location of Lua file loader that's used to load core scripts to be used.
  constexpr char LOADER_SCRIPT_PATH[] = "load.lua";
  /// The entry file that is run when a project is loaded.
  constexpr char PROJECT_ENTRYPOINT[] = "main.lua";

  /// The Lua state that stores all of the loaded script functions, variables, etc.
  extern sol::state lua_state;
  /// The filepath of the currently loaded project.
  extern std::string lua_file;

  /// Creates the Lua functions and usertypes that interface with TermEngine.
  void InitInterface();

  /// Selects the project script to execute, or the "No Program" script if one is not set.
  void InitScript();

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
  /**
   * @param[in] timestep The timestep for this frame, i.e. the amount of time since the last frame ticked.
   */
  void OnLoop(const float& timestep);

  /// Runs the "Quit" Lua function, which runs game code before closing TermEngine.
  /**
   * @returns A boolean value, indicating if TermEngine can close. This can be used by scripters to ask the player before closing, or to run clean-up code.
   */
  bool OnQuit();
}

#endif // ! SCRIPTING_INTERFACE_H
