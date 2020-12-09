#include "ScriptingInterface.h"
#include "../logging/Logger.h"
#include "../glyphs/GlyphSet.h"
#include "../scenes/SceneManager.h"
#include "../utility/Color.h"

namespace term_engine::scripting {
  void InitInterface() {
    sol::usertype<glm::vec2> vec2_type = lua_state.new_usertype<glm::vec2>(
      "vec2",
      sol::call_constructor, sol::constructors<glm::vec2(), glm::vec2(const float&), glm::vec2(const float&, const float&)>(),
      "x", &glm::vec2::x,
      "y", &glm::vec2::y);

    sol::usertype<Color> color_type = lua_state.new_usertype<Color>(
      "Color",
      sol::constructors<Color(), Color(const float&, const float&, const float&, const float&)>(),
      "r", &Color::r_,
      "g", &Color::g_,
      "b", &Color::b_,
      "a", &Color::a_);

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
      "clear", &glyphs::GlyphSet::ClearAll,
      "refresh", &glyphs::GlyphSet::Dirty);

    sol::usertype<scenes::Scene> scene_type = lua_state.new_usertype<scenes::Scene>(
      "Scene",
      sol::constructors<scenes::Scene()>(),
      "getGlyphSet", &(scenes::Scene::GetGlyphSet));

    lua_state["my_func"] = []() {
      logging::logger->info("Hello World!");
    };
    lua_state["activeScene"] = &(*scenes::active_scene_);
  }

  void Boot() {

  }

  void Run(const std::string& filename) {
    try {
      auto result = lua_state.script_file("resources/scripts/test.lua", [](lua_State* state, sol::protected_function_result pfr) {
        sol::error err = pfr;
        logging::logger->error("Received Lua error: {}", err.what());

        return pfr;
        });
    }
    catch (const std::exception& err) {
      logging::logger->error("A scripting error occurred!\nFile: {}\nError: {}", filename, err.what());
    }
  }

  sol::state lua_state;
}
