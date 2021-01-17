/// @author James Holtom

#ifndef INIT_H
#define INIT_H

#include "SDLUtils.h"
#include "GLUtils.h"
#include "FTUtils.h"

namespace term_engine {
  /// Initialises SDL.
  /**
   * @returns Was SDL successfully initialised?
   */
  int InitSDL();

  /// Prepares OpenGL for use.
  /**
   * @returns Was OpenGL successfully initialised?
   */
  int InitGL();

  /// Initialises GLEW.
  /**
   * GLEW needs to be called *after* the window has been initialised.
   * 
   * @returns Was GLEW successfully initialised?
   */
  int InitGLEW();

  /// Initialises FreeType.
  /**
   * @param[in,out] library An instance of the FreeType library to initialise.
   * @returns Was FreeType successfully initialised?
   */
  int InitFreeType(FT_Library* library);

  /// Shuts down SDL.
  void CleanUpSDL();

  /// Shuts down FreeType.
  /**
   * @param[in,out] library An instance of the FreeType library to shut down.
   * @returns Was FreeType successfully shut down?
   */
  void CleanUpFreeType(const FT_Library& library);
}

#endif // ! INIT_H