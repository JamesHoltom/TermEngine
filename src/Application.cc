#include <string>
#include "Application.h"
#include "events/InputManager.h"
#include "events/Listener.h"
#include "scripting/ScriptingInterface.h"
#include "system/CLArguments.h"
#include "system/FPSManager.h"
#include "usertypes/EventListener.h"
#include "usertypes/GameScene.h"
#include "usertypes/game_objects/BaseObject.h"
#include "usertypes/resources/BaseResource.h"
#include "utility/AudioUtils.h"
#include "utility/FTUtils.h"
#include "utility/GLUtils.h"
#include "utility/ImGuiUtils.h"
#include "utility/SDLUtils.h"
#include "utility/SpdlogUtils.h"

namespace term_engine {
  void Init()
  {
    utility::InitLogger();

    if (!utility::InitSDL() || !utility::InitFreeType() || !utility::InitAudio())
    {
      utility::logger->critical("Failed to initialise SDL/FT/MA!");

      exit(2);
    }

    SDL_StopTextInput();

    utility::InitGL();

    if (!utility::InitImGui())
    {
      exit(3);
    }

    events::InitList();
    system::InitFPS();

    scripting::SetNextProject(system::scriptPath.string());
    InitProject();

    utility::logger->debug("Finished init!");
  }

  void InitProject()
  {
    events::Init();
    scripting::Setup();
  }

  void CleanUp()
  {
    CleanUpProject();

    utility::CleanUpAudio();
    utility::CleanUpFreeType();
    utility::CleanUpSDL();
    utility::CleanUpImGui();

    utility::logger->debug("Cleaned up!");
  }

  void CleanUpProject()
  {
    events::CleanUpList();
    usertypes::ClearAllEventListeners();
    usertypes::ClearAllObjects();
    scripting::Shutdown();
    usertypes::ClearAllGameScenes();
    usertypes::CleanUpResources();
    events::CleanUp();
  }

  void Run()
  {
    bool quit = false;
    uint64_t timestep = 0;
    usertypes::Timer debug_timer;
    usertypes::Timer timestep_timer;
    debug_timer.Start();
    timestep_timer.Start();

    system::SetTargetFPS(system::DEFAULT_FPS);

    if (!scripting::OnInit())
    {
      quit = true;
    }

    while (!quit)
    {
      if (!scripting::next_project_path.empty())
      {
        CleanUpProject();
        InitProject();

        if (!scripting::OnInit())
        {
          quit = true;
          break;
        }
      }

      // Using the side-effect of SDL_QuitRequested() calling SDL_PumpEvents() to omit it from the below code.
      if (SDL_QuitRequested() || usertypes::quit_flag)
      {
        if (scripting::OnQuit())
        {
          quit = true;
          break;
        }
        else
        {
          usertypes::quit_flag = false;
        }
      }

      events::UpdateInputState();

      SDL_Event event;

      while (SDL_PollEvent(&event) != 0) {
        utility::ImGui_DoEvents(event);
        utility::LogKeyboardEvents(event);
        utility::LogTextInputEvents(event);
        utility::LogWindowEvents(event);

        usertypes::DoGameSceneEvents(event);
        events::DoSDLEvents(event);
      }

      for (const events::Event& event : events::event_list)
      {
        for (usertypes::EventListenerPtr& listener : usertypes::listener_list)
        {
          listener->DoEvents(event);
        }
      }

      events::CleanUpList();

      timestep = timestep_timer.GetIntervalElapsed();

      scripting::OnLoop(timestep);

      usertypes::SortObjects();
      usertypes::UpdateObjects(timestep);
      usertypes::UpdateGameScenes();

      usertypes::ClearFlaggedGameScenes();
      usertypes::ClearFlaggedEventListeners();
      usertypes::ClearFlaggedObjects();
      usertypes::ClearFlaggedResources();

      utility::ImGui_StartFrame();
      utility::ImGui_UpdateInfo();
      utility::ImGui_EndFrame();

      system::NextFrame();

      events::UpdatePrevInputState();

      system::CalculateFPS();
      system::Delay();
    }
  }
}