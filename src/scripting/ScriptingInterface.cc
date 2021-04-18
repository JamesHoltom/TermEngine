#include <memory>

#include "ScriptingInterface.h"
#include "../data/Uniform.h"
#include "../events/InputManager.h"
#include "../logging/Logger.h"
#include "../system/CLArguments.h"
#include "../system/FileFunctions.h"
#include "../system/Window.h"
#include "../utility/Color.h"

#include "../objects/BoxObject.h"
#include "../objects/OutlineBoxObject.h"
#include "../objects/TextObject.h"

namespace term_engine::scripting {
  void InitInterface() {
    lua_state.open_libraries(sol::lib::base, sol::lib::package);

    sol::usertype<glm::vec2> vec2_type = lua_state.new_usertype<glm::vec2>(
      "vec2",
      sol::call_constructor, sol::constructors<void(), void(const float&), void(const float&, const float&)>(),
      "x", &glm::vec2::x,
      "y", &glm::vec2::y);

    sol::usertype<glm::ivec2> ivec2_type = lua_state.new_usertype<glm::ivec2>(
      "ivec2",
      sol::call_constructor, sol::constructors<void(), void(const int&), void(const int&, const int&)>(),
      "x", &glm::ivec2::x,
      "y", &glm::ivec2::y);

    sol::usertype<glm::uvec2> uvec2_type = lua_state.new_usertype<glm::uvec2>(
      "uvec2",
      sol::call_constructor, sol::constructors<void(), void(const glm::uint&), void(const glm::uint&, const glm::uint&)>(),
      "x", &glm::uvec2::x,
      "y", &glm::uvec2::y);

    sol::usertype<glm::uvec3> uvec3_type = lua_state.new_usertype<glm::uvec3>(
      "uvec3",
      sol::call_constructor, sol::constructors<void(), void(const glm::uint&), void(const glm::uint&, const glm::uint&, const glm::uint&)>(),
      "x", &glm::uvec3::x,
      "y", &glm::uvec3::y,
      "z", &glm::uvec3::z);

    sol::usertype<glm::vec4> vec4_type = lua_state.new_usertype<glm::vec4>(
      "vec4",
      sol::call_constructor, sol::constructors<void(), void(const float&), void(const float&, const float&, const float&, const float&)>(),
      "x", &glm::vec4::x,
      "y", &glm::vec4::y,
      "z", &glm::vec4::z,
      "w", &glm::vec4::w);

    sol::usertype<glyphs::GlyphParams> glyph_parameters_type = lua_state.new_usertype<glyphs::GlyphParams>(
      "Glyph",
      sol::call_constructor, sol::constructors<void(const char&, const glm::vec4&, const glm::vec4&)>(),
      "character", &glyphs::GlyphParams::character_,
      "foreground_color", &glyphs::GlyphParams::foreground_color_,
      "background_color", &glyphs::GlyphParams::background_color_);

    sol::usertype<objects::BoxObject> box_object_type = lua_state.new_usertype<objects::BoxObject>(
      "Box",
      sol::no_constructor,
      "position", sol::property(&objects::BoxObject::GetPosition, &objects::BoxObject::SetPosition));
    
    sol::usertype<objects::OutlinedBoxObject> outlined_box_object_type = lua_state.new_usertype<objects::OutlinedBoxObject>(
      "OutlinedBox",
      sol::no_constructor,
      "position", sol::property(&objects::OutlinedBoxObject::GetPosition, &objects::OutlinedBoxObject::SetPosition));
    
    sol::usertype<objects::TextObject> text_object_type = lua_state.new_usertype<objects::TextObject>(
      "Text",
      sol::no_constructor,
      "position", sol::property(&objects::TextObject::GetPosition, &objects::TextObject::SetPosition),
      "text", sol::property(&objects::TextObject::GetText, &objects::TextObject::SetText));

    lua_state.create_named_table("object",
      "addBox", [=](const glm::ivec2& position, const glm::ivec2& size, const glyphs::GlyphParams& params) {
        return (objects::BoxObject*)objects::object_list.emplace_back(new objects::BoxObject(position, size, params));
      },
      "addOutlinedBox", [=](const glm::ivec2& position, const glm::ivec2& size, const glyphs::GlyphParams& params) {
        return (objects::OutlinedBoxObject*)objects::object_list.emplace_back(new objects::OutlinedBoxObject(position, size, params));
      },
      "addText", [=](const glm::ivec2& position, const glm::ivec2& size, const std::string& text, const glm::vec4& fg_color, const glm::vec4& bg_color) {
        return (objects::TextObject*)objects::object_list.emplace_back(new objects::TextObject(text, position, fg_color, bg_color, size));
      });

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
      "position", [&]() { return events::mouse_position; },
      "movement", [&]() { return events::mouse_position_delta; });
    
    lua_state.create_named_table("keyboard",
      "isDown", &events::KeyIsDown,
      "isPressed", &events::KeyIsPressed,
      "isReleased", &events::KeyIsReleased);

    lua_state.create_named_table("project",
      "load", [&](const std::string& project_path) {},
      "reload", [&]() {},
      "unload", [&]() {});

    lua_state.create_named_table("wireframe",
      "enable", &system::EnableWireframe,
      "disable", &system::DisableWireframe);

    lua_state["getFileList"] = [&](const std::string& directory) {
      return system::GetFileList(system::GetRelative(directory));
    };

    lua_state["getFolderList"] = [&](const std::string& directory) {
      return system::GetFolderList(system::GetRelative(directory));
    };

    lua_state["Init"] = [&]() {
      logging::logger->info("TermEngine has initialised!");

      return true;
    };
    lua_state["Loop"] = [&]() {};
    lua_state["Quit"] = [&]() {
      logging::logger->info("TermEngine is quitting!");

      return true;
    };
  }

  void InitScript() {
    if (system::script_path != "" && system::IsValidPath(system::script_path)) {
      logging::logger->info("Loading project...");
      Load(system::script_path);
    }
    else {
      logging::logger->info("No project to load!");
      Load(std::string(DEFAULT_SCRIPT_PATH));
    }
  }

  void CleanUp() {
    lua_state.collect_garbage();
  }

  sol::protected_function_result Run(const std::string& function_name) {
    sol::protected_function_result result;

    try {
      result = lua_state[function_name]();

      if (!result.valid()) {
        sol::error err = result;
        logging::logger->error("Received Lua error: {}", err.what());
      }
    }
    catch (const std::exception& err) {
      logging::logger->error("A scripting error occurred!\nFile: {}\nError: {}", lua_file, err.what());
    }

    return result;
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
    return (bool)Run("Init");
  }

  void OnLoop() {
    Run("Loop");
  }

  bool OnQuit() {
    return (bool)Run("Quit");
  }

  sol::state lua_state;
  std::string lua_file;
}
