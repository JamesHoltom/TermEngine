/// @author James Holtom

#ifndef SCRIPTING_INTERFACE_H
#define SCRIPTING_INTERFACE_H

#include <memory.h>
#include <string>
#include "../utility/SolUtils.h"

namespace term_engine::scripting {
  /// @brief The location of the "No Program" script to run, only if a project was not loaded.
  constexpr char DEFAULT_SCRIPT_PATH[] = "noprogram.lua";
  /// @brief The location of Lua file loader that's used to load core scripts to be used.
  constexpr char LOADER_SCRIPT_PATH[] = "load.lua";
  /// @brief The entry file that is run when a project is loaded.
  constexpr char PROJECT_ENTRYPOINT[] = "main.lua";

  /// @brief The Lua state that stores all of the loaded script functions, variables, etc.
  extern std::unique_ptr<sol::state> lua_state;
  /// @brief The filepath of the currently loaded project.
  extern std::string lua_file;

  /// @brief Creates the Lua functions and usertypes that interface with TermEngine.
  void InitInterface();

  /// @brief Selects the project script to execute, or the "No Program" script if one is not set.
  void InitScript();

  /// @brief Does garbage collection and closes the scripting interface.
  void CleanUp();

  /**
   * @brief Loads the contents of a Lua file into the state, allowing subsequent scripts to use the contents.
   * 
   * @param[in] filename The file name/path of the Lua script to load.
   */
  void Load(const std::string& filename);

  /**
   * @brief Runs the "Init" Lua function, which is used to execute game code on startup.
   * 
   * @returns A boolean value, defined by the script as if initialisation was successful.
   */
  bool OnInit();

  /**
   * @brief Runs the "Loop" Lua function, which is used to execute game code every frame.
   * 
   * @param[in] timestep The timestep for this frame, i.e. the amount of time in milliseconds (ms) since the last frame ticked.
   */
  void OnLoop(uint64_t timestep);

  /**
   * @brief Runs the "Quit" Lua function, which runs game code before closing TermEngine.
   * 
   * @returns A boolean value, indicating if TermEngine can close. This can be used by scripters to ask the player before closing, or to run clean-up code.
   */
  bool OnQuit();
}

#endif // ! SCRIPTING_INTERFACE_H
