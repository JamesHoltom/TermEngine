/// @author James Holtom

#ifndef WINDOW_H
#define WINDOW_H

#include <memory>
#include <string>
#include "../utility/SDLUtils.h"
#include "../utility/GLUtils.h"

namespace term_engine::system {
  /// @brief The default width of the window.
  constexpr int DEFAULT_WIDTH = 640;
  /// @brief The default height of the window.
  constexpr int DEFAULT_HEIGHT = 512;
  /// @brief The default clear colour to use when refreshing the window.
  constexpr glm::vec3 DEFAULT_WINDOW_CLEAR_COLOUR = glm::vec3(0.0f);

  /// @brief The OpenGL context to bind to the window and render to.
  extern SDL_GLContext context;
  /// @brief The window to display.
  extern SDL::Window window;
  /// @brief The RGB components to use for setting the clear colour, i.e. what OpenGL uses to refresh the screen after every frame.
  extern glm::vec3 window_colour;
  /// @brief Whether to use wireframe rendering or not.
  extern GLuint window_render_mode;
  /// @brief Whether the window is currently minimised or not.
  extern bool window_is_minimised;

  /**
   * @brief Prepares the window for use.
   * @details When successfully initialised, _term_engine::InitGLEW_ is called.
   * 
   * @returns Was the window successfully created?
   */
  int InitWindow();

  /// @brief Shuts the window down.
  void CleanUpWindow();

  /**
   * @brief Returns the position of the window.
   * 
   * @returns The window position.
   */
  glm::ivec2 GetWindowPosition();

  /**
   * @brief Set the position of the window.
   * 
   * @param[in] position The new position of the window.
   */
  void SetWindowPosition(const glm::ivec2& position);

  /// @brief Centers the window to the desktop.
  void CenterWindowPosition();

  /**
   * @brief Returns the size of the window.
   * 
   * @returns The window size.
   */
  glm::ivec2 GetWindowSize();

  /**
   * @brief Sets the size of the window.
   * 
   * @param[in] size The new size of the window.
   */
  void SetWindowSize(const glm::ivec2& size);

  /// @brief Sets the size of the window to fit the view.
  void ResizeWindowToView();

  /**
   * @brief Returns if the window can be resized by the player.
   * 
   * @returns If the window is resizable.
   */
  bool IsWindowResizable();

  /**
   * @brief Sets if the window can be resized by the player.
   * 
   * @param[in] flag The value to set.
   */
  void SetWindowResizable(const bool& flag);

  /**
   * @brief Returns if the window is currently minimised.
   * 
   * @returns If the window is minimised.
   */
  bool IsWindowMinimised();

  /**
   * @brief Returns if the window is currently maximised.
   * 
   * @return If the window is maximised.
   */
  bool IsWindowMaximised();

  /// @brief Minimises the window.
  void MinimiseWindow();

  /// @brief Maximises the window.
  void MaximiseWindow();

  /// @brief Restores the window to its original size.
  void RestoreWindow();

  /**
   * @brief Returns if the window is grabbing the mouse, preventing it from moving outside the window.
   * 
   * @returns If the mouse has been grabbed.
   */
  bool IsMouseGrabbed();

  /**
   * @brief Sets whether the window should grab the window or not.
   * 
   * @param[in] flag Whether the window should grab the mouse or not.
   */
  void SetMouseGrab(const bool& flag);

  /**
   * @brief Returns the clear colour of the window.
   * 
   * @returns The clear colour of the window.
   */
  glm::vec3 GetWindowClearColour();

  /**
   * @brief Sets the clear colour of the window.
   * 
   * @param[in] colour The clear colour to use.
   */
  void SetWindowClearColour(const glm::vec3& colour);

  /// @brief Clears the window, ready for the next frame.
  void ClearWindow();

  /// @brief Flips the window buffers, rendering the current frame's buffer to the screen.
  void RefreshWindow();

  /**
   * @brief Returns if vsync is enabled.
   * @details Vsync fixes the framerate to the monitor's refresh rate.
   * 
   * @returns If vsync is enabled or not.
   */
  bool IsVsyncEnabled();

  /**
   * @brief Enables/disables vsync.
   * @details If set to `true`, this function will try to enable adaptive vsync first, falling back to regular vsync if it isn't supported.
   * 
   * @param[in] flag Whether to enable or disable vsync.
   */
  void SetVsync(const bool& flag);

  /// @brief Enables wireframe rendering.
  void EnableWireframe();

  /// @brief Disables wireframe rendering.
  void DisableWireframe();
}

#endif // ! WINDOW_H
