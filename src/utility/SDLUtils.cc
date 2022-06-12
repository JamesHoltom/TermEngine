#include "SDLUtils.h"

#include "../logging/Logger.h"

namespace SDL {
  int InitSDL()
  {
    if (SDL_Init(SDL_INIT_FLAGS) != 0) {
      term_engine::logging::logger->error("Failed to fully initialise SDL!\nError: {}", SDL_GetError());

      return 1;
    }

    for (int i = 0; i < SDL_GetNumAudioDrivers(); ++i) {
      const char* driver_name = SDL_GetAudioDriver(i);
      term_engine::logging::logger->debug("Audio driver #{}: {}", i, driver_name);
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