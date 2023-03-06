/// @author James Holtom

#ifndef DEBUG_FUNCTIONS_H
#define DEBUG_FUNCTIONS_H

#include <memory>
#include <string>
#include <vector>
#include <SDL2/SDL.h>
#include "../timing/Timer.h"

namespace term_engine::utility {

  enum DebugSceneMenuOption { TIMING = 0, PTR_USAGE = 1, FLAGS = 2 };

  struct ObjectDebugInfo;

  typedef std::shared_ptr<ObjectDebugInfo> ObjectDebugInfoPtr;

  typedef std::weak_ptr<ObjectDebugInfo> ObjectDebugInfoWeakPtr;

  typedef std::vector<ObjectDebugInfoPtr> DebugInfoList;

  struct ObjectDebugInfo {
    std::string name_;
    long ptr_uses_;
    uint64_t update_time_;

    ObjectDebugInfo(const std::string& name) :
      name_(name)
    {}
  };

  extern DebugInfoList debug_info_list;
  extern DebugSceneMenuOption menu_option;
  extern size_t current_line;

  /**
   * @brief Logs any keyboard-related events in an _SDL_Event_ object.
   * 
   * @param[in] event The _SDL_Event_ object to check for keyboard-related events.
   */
  void LogKeyboardEvents(const SDL_Event& event);

  /**
   * @brief Logs any window-related events in an _SDL_Event_ object.
   * 
   * @param[in] event The _SDL_Event_ object to check for window-related events.
   */
  void LogWindowEvents(const SDL_Event& event);

  /// @brief Logs information about the currently bound VAO, such as what vertex attributes are bound, etc.
  void LogVAOData();

  /// @brief Logs information about the currently bound VBO.
  void LogVBOData();

  ObjectDebugInfoWeakPtr AddDebugInfo(const std::string& name);

  void RemoveDebugInfo(const ObjectDebugInfoWeakPtr& ptr);

  std::string GetDebugInfo();
}

#endif // ! DBEUG_FUNCTIONS_H
