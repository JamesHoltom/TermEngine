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
  struct ObjectDebugInfo;
  struct InfoItem;
  struct ObjectDebugInfoDeleter;

  /// @brief Unique pointer to a set of debugging information.
  typedef std::unique_ptr<ObjectDebugInfo, ObjectDebugInfoDeleter> ObjectDebugInfoPtr;
  /// @brief Used to store a list of sub-items in a set of debugging information.
  typedef std::vector<InfoItem> DebugInfoSubList;
  /// @brief Used to store a list of raw pointers to sets of debugging information.
  typedef std::vector<ObjectDebugInfo*> DebugInfoList;

  /// @brief Defines the types of debugging information available in a set.
  enum DebugSceneMenuOption { TIMING = 0 };

  /// @brief An item containing debugging information.
  struct InfoItem {
    /// @brief The title of the item.
    std::string title_;
    /// @brief The amount of time elapsed when running the code this item represents.
    uint64_t total_time_;
    /// @brief The indentation level when displaying the item.
    uint32_t indent_;

    /**
     * @brief Constructs the item with the given parameters.
     * 
     * @param[in] title   The title of the item.
     * @param[in] indent  The indentation level when displaying the item.
     */
    InfoItem(const std::string& title, uint32_t indent) :
      title_(title),
      total_time_(0),
      indent_(indent)
    {}

    /**
     * @brief Stringifies the item for rendering to the window.
     * 
     * @returns The item in string format.
     */
    std::string ToString() const;
  };

  /// @brief Used to hold debugging information for an object/purpose, with sub-items to measure specific parts.
  struct ObjectDebugInfo {
    /// @brief The root-level item, storing the total time taken, etc.
    InfoItem main_item_;
    /// @brief Timer to measure time intervals.
    timing::Timer timer_;
    /// @brief List of sub-items.
    DebugInfoSubList sub_list_;

    /**
     * @brief Constructs the set of debugging information.
     * 
     * @param[in] title The title of the main item.
     */
    ObjectDebugInfo(const std::string& title) :
      main_item_(title, 0)
    {}

    /**
     * @brief Adds a sub-item to the set.
     * 
     * @param[in] title   The title of the sub-item.
     * @param[in] indent  The indentation level when displaying the sub-item.
     */
    void AddSubItem(const std::string& title, uint32_t indent);

    /// @brief Starts the timer for measuring the time taken.
    void Start();

    /**
     * @brief Logs the interval of time taken into the given (sub-)item.
     * 
     * @param[in] interval_index The index of the (sub-)item to update.
     */
    void Interval(int interval_index = 0);

    /**
     * @brief Resets the interval value on the given (sub-)item.
     * 
     * @param[in] interval_index The index of the (sub-)item to reset.
     */
    void Reset(int interval_index = 0);

    /**
     * @brief Stringifies the set for rendering to the window.
     * 
     * @returns The set of items in string format.
     */
    std::string ToString() const;

    /**
     * @brief Returns the number of items in the set, including the main item.
     * 
     * @returns The number of items in the set.
     */
    uint64_t GetLineCount() const;
  };

  /// @brief Stores the list of raw pointers to sets of debugging information.
  extern DebugInfoList debug_info_list;

  /// @brief Used to delete a set of debugging information from the list when it's related object is removed.
  struct ObjectDebugInfoDeleter {
    /**
     * @brief Allows for calling a set to be deleted.
     * 
     * @param[in,out] ptr The raw pointer to the set being deleted.
     */
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

  /**
   * @brief Adds a set of debugging information to the list.
   * 
   * @param[in] title The title of the set.
   * @returns A unique pointer to the created set.
   */
  ObjectDebugInfoPtr AddDebugInfo(const std::string& title);

  /**
   * @brief Stringifies each set in the list for rendering to the window.
   * 
   * @param[in] option  The type of debugging information to display.
   * @param[in] line    The line to start the list from.
   * @returns The list of set items in string format.
   */
  std::string GetDebugInfo(DebugSceneMenuOption option, uint64_t line);

  /**
   * @brief Returns the number of sets in the list.
   * 
   * @returns The number of sets.
   */
  uint64_t GetDebugInfoCount();

  /**
   * @brief Returns the amount of item lines for each set in the list.
   * 
   * @returns The line total.
   */
  uint64_t GetDebugInfoLines();
}

#endif // ! DBEUG_FUNCTIONS_H
