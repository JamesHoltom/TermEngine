#include "SDLUtils.h"
#include "SpdlogUtils.h"

namespace term_engine::utility {
  int InitSDL()
  {
    if (SDL_Init(SDL_INIT_FLAGS) != 0)
    {
      utility::logger->error("Failed to fully initialise SDL!\nError: {}", SDL_GetError());

      return 1;
    }

    for (int i = 0; i < SDL_GetNumAudioDrivers(); ++i)
    {
      const char* driver_name = SDL_GetAudioDriver(i);
      utility::logger->debug("Audio driver #{}: {}", i, driver_name);
    }

    if (SDL_InitSubSystem(SDL_INIT_AUDIO) != 0)
    {
      utility::logger->error("Failed to initialise audio!\nError: {}", SDL_GetError());
    }

    srand(SDL_GetTicks());

    utility::logger->debug("Initialised SDL.");

    return 0;
  }

  void CleanUpSDL()
  {
    SDL_Quit();

    utility::logger->debug("Shut down SDL.");
  }
}