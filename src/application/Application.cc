#include <string>

#include "Application.h"
#include "../logging/Logger.h"
#include "../background/Background.h"
#include "../data/Uniform.h"
#include "../events/InputManager.h"
#include "../fonts/FontAtlas.h"
#include "../glyphs/Glyph.h"
#include "../objects/ObjectManager.h"
#include "../scripting/ScriptingInterface.h"
#include "../shaders/Shader.h"
#include "../system/ImageFunctions.h"
#include "../system/Window.h"
#include "../timing/FPSManager.h"
#include "../utility/SDLUtils.h"
#include "../utility/GLUtils.h"
#include "../utility/FTUtils.h"
#include "../timing/FPSManager.h"

namespace term_engine::application {
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

    data::Init();
    data::SetProjection(system::GetWindowSize());

    if (fonts::Init() > 0 || glyphs::Init() > 0 || background::Init() > 0) {
      logging::logger->error("Failed to initialise application!");

      exit(3);
    }

    system::SetWindowSize(glm::ivec2(fonts::texture_size) * glyphs::dimensions);

    timing::InitFPS();
    scripting::InitInterface();
    scripting::InitScript();

    logging::logger->debug("Finished init!");
  }

  void CleanUp()
  {
    objects::CleanUp();
    scripting::CleanUp();
    fonts::CleanUp();
    glyphs::CleanUp();
    data::CleanUp();
    events::CleanUp();
    system::CleanUpWindow();
    background::RemoveBackground();

    FT::CleanUpFreeType();
    SDL::CleanUpSDL();

    logging::logger->debug("Cleaned up!");
  }

  void Run()
  {
    bool quit = false;
    timing::Timer timestep;
    timestep.Start();

    if (!scripting::OnInit()) {
      quit = true;
    }

    while (!quit) {
      // Using the side-effect of SDL_QuitRequested() calling SDL_PumpEvents() to omit it from the below code.
      if (SDL_QuitRequested()) {
        if (scripting::OnQuit()) {
          quit = true;

          break;
        }
      }

      events::UpdateEvents();

      scripting::OnLoop(timestep.GetIntervalElapsed());

      system::ClearWindow();

      background::Render();
      objects::Render();
      glyphs::Render();

      system::RefreshWindow();

      timing::NextFrame();

      events::UpdatePrevEvents();

      timing::CalculateFPS();
      timing::Delay();
    }

    timestep.Stop();
  }
}