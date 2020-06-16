#include "ScriptManager.h"

namespace term_engine::scripting {
  ScriptManager::ScriptManager() {
    lua_state_.open_libraries(sol::lib::base);
  }

  ScriptManager::~ScriptManager() {
    
  }
}