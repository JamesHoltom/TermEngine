#include "ScriptingInterface.h"
#include "../logging/Logger.h"
#include "../events/InputManager.h"
#include "../glyphs/GlyphSet.h"
#include "../scenes/SceneManager.h"
#include "../utility/Color.h"

namespace term_engine::scripting {
  void InitInterface() {
    lua_state.open_libraries(sol::lib::base, sol::lib::package);

    sol::usertype<glm::vec2> vec2_type = lua_state.new_usertype<glm::vec2>(
      "vec2",
      sol::call_constructor, sol::constructors<glm::vec2(), glm::vec2(const float&), glm::vec2(const float&, const float&)>(),
      "x", &glm::vec2::x,
      "y", &glm::vec2::y);

    sol::usertype<glm::ivec2> ivec2_type = lua_state.new_usertype<glm::ivec2>(
      "ivec2",
      sol::call_constructor, sol::constructors<glm::ivec2(), glm::ivec2(const int&), glm::ivec2(const int&, const int&)>(),
      "x", &glm::ivec2::x,
      "y", &glm::ivec2::y);

    sol::usertype<glm::uvec2> uvec2_type = lua_state.new_usertype<glm::uvec2>(
      "uvec2",
      sol::call_constructor, sol::constructors<glm::uvec2(), glm::uvec2(const glm::uint&), glm::uvec2(const glm::uint&, const glm::uint&)>(),
      "x", &glm::uvec2::x,
      "y", &glm::uvec2::y);

    sol::usertype<glm::vec4> color_type = lua_state.new_usertype<glm::vec4>(
      "Color",
      sol::call_constructor, sol::constructors<glm::vec4(), glm::vec4(const float&, const float&, const float&, const float&)>(),
      "r", &glm::vec4::r,
      "g", &glm::vec4::g,
      "b", &glm::vec4::b,
      "a", &glm::vec4::a);

    sol::usertype<glyphs::GlyphData> glyph_data_type = lua_state.new_usertype<glyphs::GlyphData>(
      "Glyph",
      sol::no_constructor,
      "position", sol::readonly(&glyphs::GlyphData::index_),
      "character", &glyphs::GlyphData::character_,
      "foreground_color", &glyphs::GlyphData::foreground_color_,
      "background_color", &glyphs::GlyphData::background_color_);

    sol::usertype<glyphs::GlyphSet> glyph_set_type = lua_state.new_usertype<glyphs::GlyphSet>(
      "GlyphSet",
      sol::no_constructor,
      "position", sol::property(&glyphs::GlyphSet::GetPosition, &glyphs::GlyphSet::SetPosition),
      "scale", sol::property(&glyphs::GlyphSet::GetScale, &glyphs::GlyphSet::SetScale),
      "padding", sol::property(&glyphs::GlyphSet::GetPadding, &glyphs::GlyphSet::SetPadding),
      "spacing", sol::property(&glyphs::GlyphSet::GetSpacing, &glyphs::GlyphSet::SetSpacing),
      "size", sol::property(&glyphs::GlyphSet::GetSize, &glyphs::GlyphSet::SetSize),
      "count", sol::readonly_property(&glyphs::GlyphSet::GetCount),
      "get", &glyphs::GlyphSet::GetGlyph,
      "writeLine", &glyphs::GlyphSet::WriteLine,
      "clear", &glyphs::GlyphSet::ClearAll,
      "refresh", &glyphs::GlyphSet::Dirty);

    sol::usertype<scenes::Scene> scene_type = lua_state.new_usertype<scenes::Scene>(
      "Scene",
      sol::call_constructor, sol::constructors<scenes::Scene()>(),
      "getGlyphSet", &scenes::Scene::GetGlyphSet);

    lua_state["activeScene"] = std::ref(scenes::active_scene_);

    lua_state["mouseIsDown"] = &events::MouseIsDown;
    lua_state["mouseIsPressed"] = &events::MouseIsPressed;
    lua_state["mouseIsReleased"] = &events::MouseIsReleased;
    lua_state["mousePosition"] = [&]() { return events::mouse_position; };
    lua_state["mouseMovement"] = [&]() { return events::mouse_position_delta; };
    lua_state["keyIsDown"] = &events::KeyIsDown;
    lua_state["keyIsPressed"] = &events::KeyIsPressed;
    lua_state["keyIsReleased"] = &events::KeyIsReleased;
  }

  void InitScript() {
    Load("resources/scripts/init.lua");
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
      sol::protected_function_result result = lua_state.script_file(filename);

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
