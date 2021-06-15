#include "SDLUtils.h"

#include "../logging/Logger.h"

namespace SDL {
  int InitSDL()
  {
    if (SDL_Init(SDL_INIT_FLAGS) < 0) {
      term_engine::logging::logger->error("Failed to fully initialise SDL!\nError: {}", SDL_GetError());

      return 1;
    }

    if (IMG_Init(IMG_INIT_FLAGS) != IMG_INIT_FLAGS) {
      term_engine::logging::logger->error("Failed to fully initialise SDL_image!\nError: {}", IMG_GetError());

      return 1;
    }

    srand(SDL_GetTicks());

    term_engine::logging::logger->debug("Initialised SDL.");

    return 0;
  }

  void CleanUpSDL()
  {
    IMG_Quit();
    SDL_Quit();

    term_engine::logging::logger->debug("Shut down SDL.");
  }
}