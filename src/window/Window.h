/// @author James Holtom

#ifndef WINDOW_H
#define WINDOW_H

#include <memory>
#include <string>

#include "../utility/SDLUtils.h"
#include "../utility/GLUtils.h"

namespace term_engine::windows {
  /// The default width of the window.
  constexpr int DEFAULT_WIDTH = 640;
  /// The default height of the window.
  constexpr int DEFAULT_HEIGHT = 512;

  /// The OpenGL context to bind to the window and render to.
  extern SDL_GLContext context;
  /// The window to display.
  extern SDL::Window window;

  /// Prepares the window for use.
  /**
   * When successfully initialised, _term_engine::InitGLEW_ is called.
   */
  void Init();

  /// Shuts the window down.
  void CleanUp();

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
}

#endif // ! WINDOW_H
