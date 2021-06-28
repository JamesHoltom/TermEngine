#include <filesystem>

#include "ScriptingInterface.h"
#include "bindings/glmBindings.h"
#include "bindings/BackgroundBindings.h"
#include "bindings/EventBindings.h"
#include "bindings/FileBindings.h"
#include "bindings/FontBindings.h"
#include "bindings/GlyphBindings.h"
#include "bindings/ObjectBindings.h"
#include "bindings/ProjectBindings.h"
#include "bindings/UtilityBindings.h"
#include "bindings/ViewBindings.h"
#include "bindings/WindowBindings.h"
#include "../logging/Logger.h"
#include "../system/CLArguments.h"
#include "../system/FileFunctions.h"


namespace term_engine::scripting {
  void InitInterface() {
    lua_state.open_libraries(sol::lib::base, sol::lib::package, sol::lib::math, sol::lib::string);

    // Create bindings for C++ functions.
    bindings::BindGlmToState(lua_state);
    bindings::BindBackgroundToState(lua_state);
    bindings::BindEventToState(lua_state);
    bindings::BindFileToState(lua_state);
    bindings::BindFontToState(lua_state);
    bindings::BindGlyphToState(lua_state);
    bindings::BindObjectToState(lua_state);
    bindings::BindProjectToState(lua_state);
    bindings::BindUtilitiesToState(lua_state);
    bindings::BindViewToState(lua_state);
    bindings::BindWindowToState(lua_state);

    // Create bindings for the main game functions.
    lua_state["Init"] = [&]() {
      logging::logger->info("TermEngine has initialised!");

      return true;
    };
    lua_state["Loop"] = [&](const float& timestep) {};
    lua_state["Quit"] = [&]() {
      logging::logger->info("TermEngine is quitting!");

      return true;
    };
  }

  void InitScript() {
    Load(std::string(LOADER_SCRIPT_PATH));

    std::filesystem::path project_file = std::filesystem::absolute(system::script_path / std::string(PROJECT_ENTRYPOINT));

    if (system::script_path != "" && File::Exists(project_file)) {
      logging::logger->info("Loading project...");
      Load(project_file.string());
    }
    else {
      logging::logger->info("No project to load!");
      Load(std::string(DEFAULT_SCRIPT_PATH));
    }
  }

  void Load(const std::string& filename) {
    try {
      sol::protected_function_result result = lua_state.script_file(filename);

      if (result.valid()) {
        lua_file = filename;
        logging::logger->debug("Loaded Lua script {}.", filename);
      }
      else {
        sol::error err = result;
        logging::logger->error("Failed to load Lua script {}\nError: {}. ", filename, err.what());
      }
    }
    catch (const std::exception& err) {
      logging::logger->error("A loading error occurred!\nFile: {}\nError: {}", lua_file, err.what());
    }
  }

  bool OnInit() {
    bool return_value = true;

    try {
      sol::protected_function_result result = lua_state["Init"]();

      if (result.valid()) {
        return_value = (bool)result;
      }
      else {
        sol::error err = result;
        logging::logger->error("Received Lua error on init: {}", err.what());
      }
    }
    catch (const std::exception& err) {
      logging::logger->error("A scripting error occurred!\nFile: {}\nError: {}", lua_file, err.what());
    }

    return return_value;
  }

  void OnLoop(const float& timestep) {
    try {
      sol::protected_function_result result = lua_state["Loop"](timestep);

      if (!result.valid()) {
        sol::error err = result;
        logging::logger->error("Received Lua error on loop: {}", err.what());
      }
    }
    catch (const std::exception& err) {
      logging::logger->error("A scripting error occurred!\nFile: {}\nError: {}", lua_file, err.what());
    }
  }

  bool OnQuit() {
    bool return_value = true;

    try {
      sol::protected_function_result result = lua_state["Quit"]();

      if (result.valid()) {
        return_value = (bool)result;
      }
      else {
        sol::error err = result;
        logging::logger->error("Received Lua error on quit: {}", err.what());
      }
    }
    catch (const std::exception& err) {
      logging::logger->error("A scripting error occurred!\nFile: {}\nError: {}", lua_file, err.what());
    }

    return return_value;
  }

  sol::state lua_state;
  std::string lua_file;
}
