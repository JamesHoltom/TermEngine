/// @author James Holtom

#ifndef DEBUG_FUNCTIONS_H
#define DEBUG_FUNCTIONS_H

#include <string>
#include <SDL2/SDL.h>

namespace term_engine::debug {
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
}

#endif // ! DBEUG_FUNCTIONS_H
