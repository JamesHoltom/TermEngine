#include "SDLUtils.h"

#include "../logging/Logger.h"

namespace SDL {
  int InitSDL()
  {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
      term_engine::logging::logger->error("Failed to initialise SDL!\nError: {}", SDL_GetError());

      return 1;
    }

    srand(SDL_GetTicks());

    term_engine::logging::logger->debug("Initialised SDL.");

    return 0;
  }

  void CleanUpSDL()
  {
    SDL_Quit();

    term_engine::logging::logger->debug("Shut down SDL.");
  }
}