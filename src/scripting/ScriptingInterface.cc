#include <filesystem>

#include "ScriptingInterface.h"
#include "../data/Uniform.h"
#include "../events/InputManager.h"
#include "../logging/Logger.h"
#include "../system/CLArguments.h"
#include "../system/FileFunctions.h"
#include "../system/Window.h"

#include "../objects/ObjectManager.h"

namespace term_engine::scripting {
  void InitInterface() {
    lua_state.open_libraries(sol::lib::base, sol::lib::package, sol::lib::string);

    // Create usertypes for commonly used complex types.
    sol::usertype<glm::vec2> vec2_type = lua_state.new_usertype<glm::vec2>(
      "vec2",
      sol::call_constructor, sol::constructors<void(), void(const float&), void(const float&, const float&)>(),
      "x", &glm::vec2::x,
      "y", &glm::vec2::y,
      sol::meta_function::addition, sol::overload([](const glm::vec2& lhs, const glm::vec2& rhs) { return lhs + rhs; }),
      sol::meta_function::subtraction, sol::overload([](const glm::vec2& lhs, const glm::vec2& rhs) { return lhs - rhs; }),
      sol::meta_function::multiplication, sol::overload(
        [](const glm::vec2& lhs, const glm::vec2& rhs) { return lhs * rhs; },
        [](const glm::vec2& lhs, const float& rhs) { return lhs * rhs; },
        [](const float& lhs, const glm::vec2& rhs) { return lhs * rhs; }
      ),
      sol::meta_function::division, sol::overload(
        [](const glm::vec2& lhs, const glm::vec2& rhs) { return lhs / rhs; },
        [](const glm::vec2& lhs, const float& rhs) { return lhs / rhs; }
      ),
      "floor", [](const glm::vec2& self) { return glm::floor(self); });

    sol::usertype<glm::ivec2> ivec2_type = lua_state.new_usertype<glm::ivec2>(
      "ivec2",
      sol::call_constructor, sol::constructors<void(), void(const int&), void(const int&, const int&)>(),
      "x", &glm::ivec2::x,
      "y", &glm::ivec2::y);

    sol::usertype<glm::vec3> vec3_type = lua_state.new_usertype<glm::vec3>(
      "vec3",
      sol::call_constructor, sol::constructors<void(), void(const float&), void(const float&, const float&, const float&)>(),
      "x", &glm::vec3::x,
      "y", &glm::vec3::y,
      "z", &glm::vec3::z,
      "r", &glm::vec3::r,
      "g", &glm::vec3::g,
      "b", &glm::vec3::b);

    // Create usertypes for TermEngine-specific complex types.
    sol::usertype<glyphs::GlyphParams> glyph_parameters_type = lua_state.new_usertype<glyphs::GlyphParams>(
      "Glyph",
      sol::call_constructor, sol::constructors<void(const char&, const glm::vec3&, const glm::vec3&)>(),
      "character", &glyphs::GlyphParams::character_,
      "foreground_color", &glyphs::GlyphParams::foreground_color_,
      "background_color", &glyphs::GlyphParams::background_color_);

    sol::usertype<objects::Object> box_object_type = lua_state.new_usertype<objects::Object>(
      "Object",
      sol::no_constructor,
      "name", sol::readonly_property(&objects::Object::GetName),
      "position", sol::property(&objects::Object::GetPosition, &objects::Object::SetPosition),
      "size", sol::property(&objects::Object::GetSize, &objects::Object::SetSize),
      "data", sol::readonly_property(&objects::Object::GetData));

    // Create bindings for C++ functions.
    lua_state.create_named_table("objects",
      "add", &objects::Add,
      "get", &objects::Get,
      "remove", &objects::Remove);
    
    lua_state.create_named_table("window",
      "getSize", &system::GetWindowSize,
      "setSize", &system::SetWindowSize,
      "getClearColor", &system::GetWindowClearColor,
      "setClearColor", &system::SetWindowClearColor);

    lua_state.create_named_table("mouse",
      "LEFT", SDL_BUTTON_LEFT,
      "MIDDLE", SDL_BUTTON_MIDDLE,
      "RIGHT", SDL_BUTTON_RIGHT,
      "isDown", &events::MouseIsDown,
      "isPressed", &events::MouseIsPressed,
      "isReleased", &events::MouseIsReleased,
      "position", sol::var(&events::mouse_position),
      "movement", sol::var(&events::mouse_position_delta));
    
    lua_state.create_named_table("keyboard",
      "isDown", &events::KeyIsDown,
      "isPressed", &events::KeyIsPressed,
      "isReleased", &events::KeyIsReleased);

    /* lua_state.create_named_table("project",
      "load", [&](const std::string& project_path) {},
      "reload", [&]() {},
      "unload", [&]() {}); */

    lua_state.create_named_table("wireframe",
      "enable", &system::EnableWireframe,
      "disable", &system::DisableWireframe);

    lua_state.create_named_table("fs",
      "getFileList", [&](const std::string& directory) { return system::GetFileList(File::Relative(std::filesystem::path(directory))); },
      "getFolderList", [&](const std::string& directory) { return system::GetFolderList(File::Relative(std::filesystem::path(directory))); });

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
    Load(std::string(FUNCTIONS_SCRIPT_PATH));

    std::filesystem::path project_file = system::script_path / std::string(PROJECT_ENTRYPOINT);

    if (system::script_path != "" && File::Exists(project_file)) {
      logging::logger->info("Loading project...");
      Load(project_file.string());
    }
    else {
      logging::logger->info("No project to load!");
      Load(std::string(DEFAULT_SCRIPT_PATH));
    }
  }

  void CleanUp() {
    lua_state.collect_garbage();
  }

  void Load(const std::string& filename) {
    try {
      sol::protected_function_result result = lua_state.safe_script_file(filename);

      if (result.valid()) {
        lua_file = filename;
        logging::logger->info("Loaded Lua script {}.", filename);
      }
      else {
        logging::logger->error("Failed to load Lua script {}. ", filename);
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
