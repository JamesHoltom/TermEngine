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
  /// The default clear colour to use when refreshing the window.
  constexpr glm::uvec3 DEFAULT_WINDOW_CLEAR_COLOUR = glm::uvec3(0);

  /// The OpenGL context to bind to the window and render to.
  extern SDL_GLContext context;
  /// The window to display.
  extern SDL::Window window;
  /// The RGB components to use for setting the clear colour, i.e. what OpenGL uses to refresh the screen after every frame.
  extern GLfloat window_colour_r, window_colour_g, window_colour_b;
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

  /// Returns the clear colour of the window.
  /**
   * @returns The clear colour of the window.
   */
  glm::vec3 GetWindowClearColour();

  /// Sets the clear colour of the window.
  /**
   * @param[in] colour The clear colour to use.
   */
  void SetWindowClearColour(const glm::vec3& colour);

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