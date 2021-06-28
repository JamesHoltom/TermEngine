/// @author James Holtom

#ifndef WINDOW_H
#define WINDOW_H

#include <memory>
#include <string>

#include "../utility/SDLUtils.h"
#include "../utility/GLUtils.h"

namespace term_engine::system {
  /// The default width of the window.
  constexpr int DEFAULT_WIDTH = 640;
  /// The default height of the window.
  constexpr int DEFAULT_HEIGHT = 512;
  /// The default clear color to use when refreshing the window.
  constexpr glm::uvec3 DEFAULT_WINDOW_CLEAR_COLOR = glm::uvec3(32, 128, 255);

  /// The OpenGL context to bind to the window and render to.
  extern SDL_GLContext context;
  /// The window to display.
  extern SDL::Window window;
  /// The RGB components to use for setting the clear color, i.e. what OpenGL uses to refresh the screen after every frame.
  extern GLfloat window_color_r, window_color_g, window_color_b;
  /// Whether to use wireframe rendering or not.
  extern GLuint window_render_mode;

  /// Prepares the window for use.
  /**
   * When successfully initialised, _term_engine::InitGLEW_ is called.
   * @returns Was the window successfully created?
   */
  int InitWindow();

  /// Shuts the window down.
  void CleanUpWindow();

  /// Returns the size of the window.
  /**
   * @returns The size of the window.
   */
  glm::ivec2 GetWindowSize();

  /// Sets the size of the window.
  /**
   * @param[in] size The new size of the window.
   */
  void SetWindowSize(const glm::ivec2& size);

  /// Sets the size of the window to fit the view.
  void ResizeWindowToView();

  /// Returns the clear color of the window.
  /**
   * @returns The clear color of the window.
   */
  glm::uvec3 GetWindowClearColor();

  /// Sets the clear color of the window.
  /**
   * @param[in] color The clear color to use.
   */
  void SetWindowClearColor(const glm::uvec3& color);

  /// Clears the window, ready for the next frame.
  void ClearWindow();

  /// Flips the window buffers, rendering the current frame's buffer to the screen.
  void RefreshWindow();

  /// Enables wireframe rendering.
  void EnableWireframe();

  /// Disables wireframe rendering.
  void DisableWireframe();
}

#endif // ! WINDOW_H
