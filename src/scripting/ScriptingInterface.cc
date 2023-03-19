#include <filesystem>
#include "ScriptingInterface.h"
#include "bindings/glmBindings.h"
#include "bindings/CoreBindings.h"
#include "bindings/ObjectBindings.h"
#include "bindings/UtilityBindings.h"
#include "../system/CLArguments.h"
#include "../system/FileFunctions.h"
#include "../utility/SpdlogUtils.h"


namespace term_engine::scripting {
  std::unique_ptr<sol::state> lua_state;
  std::string lua_file;

  void InitInterface()
  {
    lua_state = std::make_unique<sol::state>();
    lua_state->open_libraries(sol::lib::base, sol::lib::package, sol::lib::math, sol::lib::string, sol::lib::table);

    // Create bindings for C++ functions.
    bindings::BindGlmToState(*lua_state);
    bindings::BindCoreToState(*lua_state);
    bindings::BindObjectsToState(*lua_state);
    bindings::BindUtilitiesToState(*lua_state);
  }

  void InitScript()
  {
    // Add the root & vendor to the Lua path, so that projects can "require()" files relative to these folders.
    std::string packagePath = (*lua_state)["package"]["path"];
    const std::string rootDirectory = system::GetRootPath() / "lua";

    packagePath += ";" +
                   rootDirectory + "/vendor/?.lua;" + 
                   rootDirectory + "/vendor/?/init.lua;" + 
                   rootDirectory + "/vendor/lunajson/?.lua;" + 
                   rootDirectory + "/vendor/lunajson/?/init.lua;" + 
                   rootDirectory + "/?.lua;" +
                   rootDirectory + "/?/init.lua";

    std::filesystem::path project_file = system::SearchForProjectPath(system::scriptPath / PROJECT_ENTRYPOINT);

    if (project_file != "")
    {
      // Add the project to the Lua path, so that projects can "require()" files relative to the project folder.
      system::project_path = project_file.parent_path();

      packagePath += std::string(";") +
                     system::project_path.string() + "/?.lua;" +
                     system::project_path.string() + "/?/init.lua";
    }

    (*lua_state)["package"]["path"] = packagePath;
    utility::logger->debug("Lua path: {}", std::string((*lua_state)["package"]["path"]));

    Load(rootDirectory + "/" + LOADER_SCRIPT_PATH);

    if (project_file != "")
    {
      utility::logger->info("Loading project...");
      Load(project_file.string());
    }
    else
    {
      utility::logger->info("No project to load!");
      Load(rootDirectory + "/" + DEFAULT_SCRIPT_PATH);
    }
  }

  void CleanUp()
  {
    lua_state->collect_gc();
    lua_state->collect_gc();

    lua_state.reset();
  }

  void Load(const std::string& filename)
  {
    try {
      sol::protected_function_result result = lua_state->script_file(filename);

      if (result.valid())
      {
        lua_file = filename;
        utility::logger->debug("Loaded Lua script {}.", filename);
      }
      else
      {
        sol::error err = result;
        utility::logger->error("Failed to load Lua script {}\nError: {}. ", filename, err.what());
      }
    }
    catch (const std::exception& err) {
      utility::logger->error("A loading error occurred!\nFile: {}\nError: {}", lua_file, err.what());
    }
  }

  bool OnInit()
  {
    bool return_value = true;

    try {
      sol::protected_function_result result = (*lua_state)["Init"]();

      if (result.valid())
      {
        return_value = (bool)result;
      }
      else
      {
        sol::error err = result;
        utility::logger->error("Received Lua error on init: {}", err.what());
      }
    }
    catch (const std::exception& err) {
      utility::logger->error("A scripting error occurred!\nFile: {}\nError: {}", lua_file, err.what());
    }

    return return_value;
  }

  void OnLoop(uint64_t timestep)
  {
    try {
      sol::protected_function_result result = (*lua_state)["Loop"](timestep);

      if (!result.valid())
      {
        sol::error err = result;
        utility::logger->error("Received Lua error on loop: {}", err.what());
      }
    }
    catch (const std::exception& err) {
      utility::logger->error("A scripting error occurred!\nFile: {}\nError: {}", lua_file, err.what());
    }
  }

  bool OnQuit()
  {
    bool return_value = true;

    try {
      sol::protected_function_result result = (*lua_state)["Quit"]();

      if (result.valid())
      {
        return_value = (bool)result;
      }
      else
      {
        sol::error err = result;
        utility::logger->error("Received Lua error on quit: {}", err.what());
      }
    }
    catch (const std::exception& err) {
      utility::logger->error("A scripting error occurred!\nFile: {}\nError: {}", lua_file, err.what());
    }

    return return_value;
  }
}
