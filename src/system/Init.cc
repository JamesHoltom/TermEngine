#include <stdlib.h>

#include "Init.h"
#include "Window.h"
#include "../data/Uniform.h"
#include "../events/InputManager.h"
#include "../scripting/ScriptingInterface.h"
#include "../timing/FPSManager.h"
#include "../utility/SDLUtils.h"
#include "../utility/GLUtils.h"
#include "../utility/FTUtils.h"
#include "../utility/LoggingUtils.h"
#include "../logging/Logger.h"

namespace term_engine::system {
  void Init()
  {
    logging::InitLogger();

    if (SDL::InitSDL() > 0 || GL::InitGL() > 0 || FT::InitFreeType() > 0) {
      logging::logger->critical("Failed to initialise SDL/GL/FT!");

      exit(2);
    }

    if (system::InitWindow() > 0) {
      logging::logger->critical("Failed to initialise window!");

      exit(2);
    }

    if (GL::InitGLEW() > 0) {
      logging::logger->critical("Failed to initialise GLEW!");

      exit(2);
    }

    events::Init();

    data::InitUBO();
    data::SetProjection(GetWindowSize());

    timing::InitFPS();
    scripting::InitInterface();
  }

  void CleanUp()
  {
    data::CleanUpUBO();
    events::CleanUp();
    system::CleanUpWindow();

    FT::CleanUpFreeType();
    SDL::CleanUpSDL();
  }
}
