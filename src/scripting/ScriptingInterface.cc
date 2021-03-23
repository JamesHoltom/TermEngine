#include "ScriptingInterface.h"
#include "../data/Uniform.h"
#include "../drawing/DrawFunctions.h"
#include "../events/InputManager.h"
#include "../glyphs/GlyphSet.h"
#include "../logging/Logger.h"
#include "../scenes/SceneManager.h"
#include "../system/CLArguments.h"
#include "../system/FileFunctions.h"
#include "../system/Window.h"
#include "../utility/Color.h"

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

    sol::usertype<glyphs::GlyphData> glyph_data_type = lua_state.new_usertype<glyphs::GlyphData>(
      "Glyph",
      sol::no_constructor,
      "position", sol::readonly(&glyphs::GlyphData::index_),
      "character", &glyphs::GlyphData::character_,
      "foreground_color", &glyphs::GlyphData::foreground_color_,
      "background_color", &glyphs::GlyphData::background_color_);

    lua_state.create_named_table("window",
      "getSize", &system::GetWindowSize,
      "setSize", &system::SetWindowSize,
      "getClearColor", &system::GetWindowClearColor,
      "setClearColor", &system::SetWindowClearColor);

    lua_state.create_named_table("glyphSet",
      "getPosition", &data::GetPosition,
      "setPosition", &data::SetPosition,
      "getPadding", &data::GetPadding,
      "setPadding", &data::SetPadding,
      "getSpacing", &data::GetSpacing,
      "setSpacing", &data::SetSpacing,
      "getSize", [&]() { return scenes::active_scene_->GetGlyphSet()->GetSize(); },
      "getSize", [&](const glm::uvec2& size) { return scenes::active_scene_->GetGlyphSet()->SetSize(size); },
      "glyph", lua_state.create_table_with(
        "count", [&]() { return scenes::active_scene_->GetGlyphSet()->GetCount(); },
        "resetPositions", [&]() { return scenes::active_scene_->GetGlyphSet()->ResetAllPositions(); },
        "isDirty", [&]() { return scenes::active_scene_->GetGlyphSet()->IsDirty(); }
      ),
      "font", lua_state.create_table_with(
        "getFontSize", [&]() { return scenes::active_scene_->GetFont()->GetFontSize(); },
        "getPixelSize", [&]() { return scenes::active_scene_->GetFont()->GetTextureSize(); }
      ));

    lua_state.create_named_table("draw",
      "text", [&](const std::string& text, const glm::ivec2& start_pos, const glm::ivec2& end_pos, const glm::vec4& fg_color, const glm::vec4& bg_color) {
        drawing::WriteText(*(scenes::active_scene_->GetGlyphSet()), start_pos, end_pos, text, fg_color, bg_color);
      },
      "box", [&](const char& character, const glm::ivec2& start_pos, const glm::ivec2& end_pos, const glm::vec4& foreground_color, const glm::vec4& background_color) {
        glyphs::GlyphParams params(character, glm::vec2(0.0f), foreground_color, background_color);
        drawing::DrawBox(*(scenes::active_scene_->GetGlyphSet()), start_pos, end_pos, params);
      },
      "outlinedBox", [&](const char& character, const glm::ivec2& start_pos, const glm::ivec2& end_pos, const glm::vec4& foreground_color, const glm::vec4& background_color) {
        glyphs::GlyphParams params(character, glm::vec2(0.0f), foreground_color, background_color);
        drawing::DrawOutlinedBox(*(scenes::active_scene_->GetGlyphSet()), start_pos, end_pos, params);
      });

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
