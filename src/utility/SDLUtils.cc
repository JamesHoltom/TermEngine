#include "SDLUtils.h"
#include "SpdlogUtils.h"

namespace term_engine::utility {
  bool InitSDL()
  {
    if (SDL_Init(SDL_INIT_FLAGS) != 0)
    {
      logger->error("Failed to fully initialise SDL!\nError: {}", SDL_GetError());

      return false;
    }

    for (int i = 0; i < SDL_GetNumAudioDrivers(); ++i)
    {
      const char* driver_name = SDL_GetAudioDriver(i);
    }

    if (SDL_InitSubSystem(SDL_INIT_AUDIO) != 0)
    {
      logger->error("Failed to initialise audio!\nError: {}", SDL_GetError());

      return false;
    }

    srand(SDL_GetTicks());

    logger->debug("Initialised SDL.");

    return true;
  }

  void CleanUpSDL()
  {
    SDL_Quit();

    logger->debug("Shut down SDL.");
  }
}