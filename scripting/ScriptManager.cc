#include "ScriptManager.h"

namespace term_engine::scripting {
  ScriptManager::ScriptManager() {
    lua_state_.open_libraries(sol::lib::base);
  }

  ScriptManager::~ScriptManager() {
    
  }

  void ScriptManager::RunInit() {
    lua_state_.script_file("scripts/init.lua");
  }

  void ScriptManager::RunTest() {
    lua_state_.script_file("scripts/test.lua");
  }
}