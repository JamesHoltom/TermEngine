/// @author James Holtom

#ifndef DEBUG_FUNCTIONS_H
#define DEBUG_FUNCTIONS_H

#include <algorithm>
#include <iomanip>
#include <memory>
#include <sstream>
#include <string>
#include <vector>
#include <SDL2/SDL.h>
#include "../timing/Timer.h"

namespace term_engine::utility {

  enum DebugSceneMenuOption { TIMING = 0 };

  struct ObjectDebugInfo;
  struct InfoItem;
  struct ObjectDebugInfoDeleter;

  typedef std::unique_ptr<ObjectDebugInfo, ObjectDebugInfoDeleter> ObjectDebugInfoPtr;

  typedef std::vector<InfoItem> DebugInfoSubList;
  typedef std::vector<ObjectDebugInfo*> DebugInfoList;

  struct InfoItem {
    std::string name_;
    uint64_t total_time_;
    uint32_t indent_;

    InfoItem(const std::string& name, uint32_t indent) :
      name_(name),
      total_time_(0),
      indent_(indent)
    {}

    std::string ToString() const;
  };

  struct ObjectDebugInfo {
    InfoItem main_item_;
    timing::Timer timer_;
    DebugInfoSubList sub_list_;

    ObjectDebugInfo(const std::string& name) :
      main_item_(name, 0)
    {}

    void AddSubItem(const std::string& name, uint32_t indent);

    void Start();

    void Interval(int interval_index = 0);

    std::string ToString() const;

    uint64_t GetLineCount() const;
  };

  extern DebugInfoList debug_info_list;

  struct ObjectDebugInfoDeleter {
    void operator()(ObjectDebugInfo* ptr) const
    {
      DebugInfoList::iterator it = std::find(debug_info_list.begin(), debug_info_list.end(), ptr);

      if (it != debug_info_list.end())
      {
        debug_info_list.erase(it);
      }

      delete ptr;
    }
  };

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

  ObjectDebugInfoPtr AddDebugInfo(const std::string& name);

  std::string GetDebugInfo(DebugSceneMenuOption option, uint64_t line);

  uint64_t GetDebugInfoCount();

  uint64_t GetDebugInfoLines();
}

#endif // ! DBEUG_FUNCTIONS_H
