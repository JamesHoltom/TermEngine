#include "SDLUtils.h"
#include "LogUtils.h"

namespace term_engine::utility {
  bool InitSDL()
  {
    if (SDL_Init(SDL_INIT_FLAGS) != 0)
    {
      LogError("Failed to fully initialise SDL!\nError: {}", SDL_GetError());

      return false;
    }

    srand(SDL_GetTicks());

    LogDebug("Initialised SDL.");

    return true;
  }

  void CleanUpSDL()
  {
    SDL_Quit();

    LogDebug("Shut down SDL.");
  }
}