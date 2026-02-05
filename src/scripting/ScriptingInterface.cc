#include "ScriptingInterface.h"
#include "bindings/glmBindings.h"
#include "bindings/CoreBindings.h"
#include "bindings/ObjectBindings.h"
#include "bindings/UtilityBindings.h"
#include "../system/FileFunctions.h"
#include "../utility/LogUtils.h"

namespace term_engine::scripting {
  int Print(lua_State* L)
  {
    std::string str = lua_tostring(L, 1);
    utility::LogInfo(str);

    return 1;
  }

  void SetNextProject(const std::string& filepath)
  {
    std::filesystem::path project_path = system::SearchForProjectPath(filepath);

    if (std::filesystem::exists(project_path))
    {
      next_project_path = project_path;

      utility::LogInfo("Loading project...");
    }
    else
    {
      next_project_path = std::filesystem::current_path() / "projects" / "noprogram";

      utility::LogInfo("No project to load!");
    }
  }

  void ReloadProject()
  {
    next_project_path = project_path;
  }

  void LoadFile(const std::filesystem::path& filepath)
  {
    try {
      sol::protected_function_result result = lua_state->script_file(filepath.string());

      if (result.valid())
      {
        utility::LogDebug("Loaded Lua script {}.", filepath.string());
      }
      else
      {
        sol::error err = result;
        utility::LogError("Failed to load Lua script {}\nError: {}. ", filepath.string(), err.what());
      }
    }
    catch (const std::exception& err) {
      utility::LogError("A loading error occurred!\nFile: {}\nError: {}", filepath.string(), err.what());
    }
  }

  void Setup()
  {
    if (lua_state != nullptr)
    {
      utility::LogError("Lua state is set up when it shouldn't be!");

      return;
    }

    project_path = next_project_path;
    next_project_path.clear();

    lua_state = std::make_unique<sol::state>();
    lua_state->open_libraries(sol::lib::base, sol::lib::package, sol::lib::math, sol::lib::string, sol::lib::table, sol::lib::utf8);

    // Add the root, vendor & project to the Lua path, so that scripts can "require()" files relative to these folders.
#if defined(__linux__)
    const std::string thisPath = std::filesystem::canonical("/proc/self/exe").parent_path().string() + "/lua";
#elif defined(_WIN32) || defined (_WIN64)
    const std::filesystem::path thisPath = GetModuleFileNameA(NULL);
#endif
    std::string package_path = (*lua_state)["package"]["path"];
    package_path += ";" +
      thisPath + "/vendor/?.lua;" + 
      thisPath + "/vendor/?/init.lua;" + 
      thisPath + "/vendor/lunajson/?.lua;" + 
      thisPath + "/vendor/lunajson/?/init.lua;" + 
      thisPath + "/?.lua;" +
      thisPath + "/?/init.lua;" +
      project_path.string() + "/?.lua;" +
      project_path.string() + "/?/init.lua";
    (*lua_state)["package"]["path"] = package_path;

    utility::LogDebug("Lua path: {}", std::string((*lua_state)["package"]["path"]));

    // Create bindings for C++ functions.
    bindings::BindGlmToState(*lua_state);
    bindings::BindCoreToState(*lua_state);
    bindings::BindUsertypesToState(*lua_state);
    bindings::BindUtilitiesToState(*lua_state);

    lua_state->set_function("print", Print);

    (*lua_state)["defaultFont"] = usertypes::LoadFont(std::string(usertypes::DEFAULT_FONT));
    (*lua_state)["defaultTextShader"] = usertypes::AddShader(std::string(usertypes::DEFAULT_TEXT_SHADER), shaders::DEFAULT_VERT_GLSL, shaders::TEXT_FRAG_GLSL, "");
    (*lua_state)["defaultBGShader"] = usertypes::AddShader(std::string(usertypes::DEFAULT_BG_SHADER), shaders::DEFAULT_VERT_GLSL, shaders::BACKGROUND_FRAG_GLSL, "");
    (*lua_state)["defaultGameScene"] = usertypes::AddGameScene(std::string(usertypes::DEFAULT_GAME_SCENE_NAME));
    (*lua_state)["defaultWindow"] = usertypes::AddDefaultGameWindow();

    LoadFile(thisPath + std::string(LOADER_SCRIPT_PATH));
    LoadFile(project_path / std::string(PROJECT_ENTRYPOINT));

    utility::LogInfo("Loaded project.");
  }

  void Shutdown()
  {
    if (lua_state != nullptr)
    {
      lua_state->collect_gc();
      lua_state->collect_gc();

      lua_state.reset();
    }

    utility::LogInfo("Shut down project.");
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
        utility::LogError("Received Lua error on init: {}", err.what());
      }
    }
    catch (const std::exception& err) {
      utility::LogError("A scripting error occurred!\nProject: {}\nError: {}", project_path.string(), err.what());
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
        utility::LogError("Received Lua error on loop: {}", err.what());
      }
    }
    catch (const std::exception& err) {
      utility::LogError("A scripting error occurred!\nProject: {}\nError: {}", project_path.string(), err.what());
    }
  }

  void OnQuit()
  {
    try {
      sol::protected_function_result result = (*lua_state)["Quit"]();

      if (!result.valid())
      {
        sol::error err = result;
        utility::LogError("Received Lua error on quit: {}", err.what());
      }
    }
    catch (const std::exception& err) {
      utility::LogError("A scripting error occurred!\nProject: {}\nError: {}", project_path.string(), err.what());
    }
  }
}
