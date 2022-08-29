#include <string>
#include "Application.h"
#include "../logging/Logger.h"
#include "../utility/AudioUtils.h"
#include "../utility/SDLUtils.h"
#include "../utility/GLUtils.h"
#include "../utility/FTUtils.h"
#include "../data/Uniform.h"
#include "../events/InputManager.h"
#include "../events/Listener.h"
#include "../objects/Object.h"
#include "../resources/Audio.h"
#include "../resources/Background.h"
#include "../resources/FontAtlas.h"
#include "../resources/Window.h"
#include "../scripting/ScriptingInterface.h"
#include "../system/DebugFunctions.h"
#include "../timing/FPSManager.h"
#include "../view/View.h"

namespace term_engine::application {
  void Init()
  {
    logging::InitLogger();

    if (SDL::InitSDL() > 0 || GL::InitGL() > 0 || FT::InitFreeType() > 0)
    {
      logging::logger->critical("Failed to initialise SDL/GL/FT!");
      exit(2);
    }

    if (system::InitWindow() > 0)
    {
      logging::logger->critical("Failed to initialise window!");
      exit(2);
    }

    if (GL::InitGLEW() > 0)
    {
      logging::logger->critical("Failed to initialise GLEW!");
      exit(2);
    }

    audio::Init();

    events::Init();

    data::Init();
    data::SetProjection(system::GetWindowSize());

    background::Init();

    if (!fonts::Init())
    {
      logging::logger->error("Failed to initialise application!");
      exit(3);
    }

    views::Init();
    system::ResizeWindowToView();

    timing::InitFPS();
    scripting::InitInterface();
    scripting::InitScript();

    logging::logger->debug("Finished init!");
  }

  void CleanUp()
  {
    scripting::CleanUp();
    objects::Object::CleanUp();
    events::EventListener::CleanUp();
    resources::Audio::CleanUp();
    fonts::CleanUp();
    views::CleanUp();
    data::CleanUp();
    events::CleanUp();
    audio::CleanUp();
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

    timing::SetTargetFPS(timing::DEFAULT_FPS);

    if (!scripting::OnInit())
    {
      quit = true;
    }

    while (!quit)
    {
      // Using the side-effect of SDL_QuitRequested() calling SDL_PumpEvents() to omit it from the below code.
      if (SDL_QuitRequested())
      {
        if (scripting::OnQuit())
        {
          quit = true;
          break;
        }
      }

      events::UpdateEvents();

      SDL_Event event;

      while (SDL_PollEvent(&event) != 0) {
        debug::LogKeyboardEvents(event);
        debug::LogWindowEvents(event);

        if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
        {
          data::SetProjection(glm::ivec2(event.window.data1, event.window.data2));
        }

        events::EventListener::DoSDLEvents(event);
      }

      scripting::OnLoop(timestep.GetIntervalElapsed());

      bool canRender = true;
      if (system::IsWindowMinimised())
      {
        canRender = false;
      }

      if (canRender)
      {
        system::ClearWindow();

        background::Render();
        views::Render();

        system::RefreshWindow();
      }

      timing::NextFrame();

      events::UpdatePrevEvents();

      timing::CalculateFPS();
      timing::Delay();
    }

    timestep.Stop();
  }
}