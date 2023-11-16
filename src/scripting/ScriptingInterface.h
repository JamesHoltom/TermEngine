/// @author James Holtom

#ifndef SCRIPTING_INTERFACE_H
#define SCRIPTING_INTERFACE_H

#include <filesystem>
#include <memory.h>
#include <string>
#include "../utility/SolUtils.h"

namespace term_engine::scripting {
  /// @brief The location of Lua file loader that's used to load core scripts to be used.
  constexpr char LOADER_SCRIPT_PATH[] = "/load.lua";
  /// @brief The entry file that is run when a project is loaded.
  constexpr char PROJECT_ENTRYPOINT[] = "main.lua";

  /// @brief The Lua state that stores all of the loaded script functions, variables, etc.
  inline std::unique_ptr<sol::state> lua_state = nullptr;
  /// @brief The filepath of the currently loaded project.
  inline std::filesystem::path project_path = "";
  /// @brief If set, will cause the engine to unload the current project and load the next one.
  inline std::filesystem::path next_project_path = "";

  /**
   * @brief Override for Lua's "print()" function, to print text to the console output with formatting.
   * 
   * @param[in] L The Lua state.
   * @returns 0 if successful, or >0 if there was an issue.
   */
  int Print(lua_State* L);

  /**
   * @brief Loads the contents of a project's "main.lua" file into the state.
   * 
   * @param[in] filepath The filepath of the Lua script to load.
   */
  void SetNextProject(const std::string& filepath);

  /// @brief Reloads the current project's "main.lua" file.
  void ReloadProject();

  /**
   * @brief Loads and executes the contents of the Lua script at the given filepath.
   * 
   * @param[in] filepath The file path to the Lua script.
   */
  void LoadFile(const std::filesystem::path& filepath);

  /// @brief Sets up the Lua state and prepares it for use.
  void Setup();

  /// @brief Does garbage collection and closes the scripting interface.
  void Shutdown();

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
  void OnQuit();
}

#endif // ! SCRIPTING_INTERFACE_H
