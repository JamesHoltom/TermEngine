#include <string>
#include "Application.h"
#include "events/InputManager.h"
#include "events/Listener.h"
#include "objects/BaseObject.h"
#include "objects/GameScene.h"
#include "scripting/ScriptingInterface.h"
#include "timing/FPSManager.h"
#include "utility/AudioUtils.h"
#include "utility/DebugUtils.h"
#include "utility/FTUtils.h"
#include "utility/GLUtils.h"
#include "utility/SDLUtils.h"
#include "utility/SpdlogUtils.h"

namespace term_engine {
  void Init()
  {
    utility::InitLogger();

    if (utility::InitSDL() > 0 || utility::InitFreeType() > 0)
    {
      utility::logger->critical("Failed to initialise SDL/FT!");
      exit(2);
    }

    utility::InitGL();
    utility::InitAudio();
    events::Init();
    events::InitQueue();
    timing::InitFPS();

    scripting::InitInterface();
    scripting::InitScript();

    utility::logger->debug("Finished init!");
  }

  void CleanUp()
  {
    objects::CleanUpObjects();
    events::CleanUpQueue();
    scripting::CleanUp();
    events::CleanUp();

    utility::CleanUpFreeType();
    utility::CleanUpSDL();

    utility::logger->debug("Cleaned up!");
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
        utility::LogKeyboardEvents(event);
        utility::LogWindowEvents(event);
        events::DoSDLEvents(event);

        if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE)
        {
          objects::MarkGameSceneForRemoval(event.window.windowID);
        }
      }

      scripting::OnLoop(timestep.GetIntervalElapsed());

      objects::SortObjects();

      for (objects::ObjectPtr& object : objects::object_list_)
      {
        object->Update();
      }

      objects::ClearFlaggedGameScenes();

      timing::NextFrame();

      events::UpdatePrevEvents();

      timing::CalculateFPS();
      timing::Delay();
    }

    timestep.Stop();
  }
}