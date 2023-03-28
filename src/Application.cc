#include <string>
#include "Application.h"
#include "events/InputManager.h"
#include "events/Listener.h"
#include "objects/BaseObject.h"
#include "objects/GameScene.h"
#include "rendering/FontAtlas.h"
#include "scripting/ScriptingInterface.h"
#include "timing/FPSManager.h"
#include "utility/AudioUtils.h"
#include "utility/FTUtils.h"
#include "utility/GLUtils.h"
#include "utility/SDLUtils.h"
#include "utility/SpdlogUtils.h"

namespace term_engine {
  utility::ObjectDebugInfoPtr app_debug_info;

  void Init()
  {
    utility::InitLogger();

    if (!utility::InitSDL() || !utility::InitFreeType() || !utility::InitAudio())
    {
      utility::logger->critical("Failed to initialise SDL/FT/MA!");
      exit(2);
    }

    utility::InitGL();
    events::Init();
    events::InitList();
    timing::InitFPS();

    scripting::InitInterface();
    scripting::InitScript();

    (*scripting::lua_state)["defaultScene"] = objects::AddGameScene(std::string(objects::DEFAULT_GAME_SCENE_NAME));

    app_debug_info = utility::AddDebugInfo("[Main Loop]");
    app_debug_info->AddSubItem("Events", 1);
    app_debug_info->AddSubItem("Script", 1);
    app_debug_info->AddSubItem("Update", 1);
    app_debug_info->AddSubItem("Flagging", 1);
    app_debug_info->AddSubItem("Loop", 1);
    app_debug_info->AddSubItem("Remaining", 1);

    utility::logger->debug("Finished init!");
  }

  void CleanUp()
  {
    app_debug_info.reset();

    objects::CleanUpObjects();
    events::CleanUpList();
    scripting::CleanUp();
    rendering::CleanUpFontAtlas();
    events::CleanUp();

    rendering::GameWindow::CleanUpContext();

    utility::CleanUpFreeType();
    utility::CleanUpSDL();

    utility::logger->debug("Cleaned up!");
  }

  void Run()
  {
    bool quit = false;
    timing::Timer debug_timer;
    timing::Timer timestep;
    debug_timer.Start();
    timestep.Start();

    timing::SetTargetFPS(timing::DEFAULT_FPS);

    if (!scripting::OnInit())
    {
      quit = true;
    }

    while (!quit)
    {
      app_debug_info->Start();

      // Using the side-effect of SDL_QuitRequested() calling SDL_PumpEvents() to omit it from the below code.
      if (SDL_QuitRequested() || objects::GameScene::quit_flag_)
      {
        if (scripting::OnQuit())
        {
          quit = true;
          break;
        }
        else
        {
          objects::GameScene::quit_flag_ = false;
        }
      }

      events::UpdateEvents();
      events::CleanUpList();

      SDL_Event event;

      while (SDL_PollEvent(&event) != 0) {
        utility::LogKeyboardEvents(event);
        utility::LogWindowEvents(event);
        events::DoSDLEvents(event);

        if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE)
        {
          objects::FlagGameSceneForRemoval(event.window.windowID);
        }
      }

      app_debug_info->Interval();

      scripting::OnLoop(timestep.GetIntervalElapsed());

      app_debug_info->Interval();

      objects::SortObjects();
      objects::UpdateObjects();

      app_debug_info->Interval();

      objects::FlagGameObjectsWithFlaggedGameScenes();
      objects::ClearFlaggedObjects();

      app_debug_info->Interval();

      timing::NextFrame();

      events::UpdatePrevEvents();

      timing::CalculateFPS();
      app_debug_info->Interval();

      timing::Delay();
      app_debug_info->Interval();
    }
  }
}