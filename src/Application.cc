#include <string>
#include "Application.h"
#include "events/InputManager.h"
#include "events/Listener.h"
#include "usertypes/game_objects/BaseObject.h"
#include "usertypes/resources/BaseResource.h"
#include "usertypes/resources/ShaderProgram.h"
#include "usertypes/GameScene.h"
#include "scripting/ScriptingInterface.h"
#include "system/FPSManager.h"
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

    utility::InitGL();
    events::Init();
    events::InitList();
    system::InitFPS();

    utility::InitContext();
    usertypes::InitDefaultShaders();
    scripting::InitInterface();
    scripting::InitScript();

    (*scripting::lua_state)["defaultScene"] = usertypes::AddGameScene(std::string(usertypes::DEFAULT_GAME_SCENE_NAME));
    (*scripting::lua_state)["defaultFont"] = usertypes::LoadFont(std::string(usertypes::DEFAULT_FONT));
    (*scripting::lua_state)["defaultTextShader"] = usertypes::GetShader(std::string(usertypes::DEFAULT_TEXT_SHADER));
    (*scripting::lua_state)["defaultBGShader"] = usertypes::GetShader(std::string(usertypes::DEFAULT_BG_SHADER));

    /* TODO: Set debug timing intervals as below:
     *  - Events
     *  - Script
     *  - Update Objects
     *  - Update Scenes
     *  - Flagging
     *  - Loop
     *  - Remaining
     */

    utility::logger->debug("Finished init!");
  }

  void CleanUp()
  {
    events::CleanUpList();
    scripting::CleanUp();
    usertypes::CleanUpObjects();
    usertypes::ClearAllGameScenes();
    usertypes::CleanUpResources();
    events::CleanUp();

    utility::CleanUpAudio();
    utility::CleanUpFreeType();
    utility::CleanUpSDL();
    utility::CleanUpImGui();
    utility::CleanUpContext();

    utility::logger->debug("Cleaned up!");
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
      // TODO: Figure out how to get execution time here.

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
      events::CleanUpList();

      SDL_Event event;

      while (SDL_PollEvent(&event) != 0) {
        utility::LogKeyboardEvents(event);
        utility::LogWindowEvents(event);

        ImGui_ImplSDL2_ProcessEvent(&event);

        utility::ImGui_DoEvents(event);
        usertypes::DoGameSceneEvents(event);
        events::DoSDLEvents(event);
      }

      utility::ImGui_StartFrame();

      if (utility::test_mode)
      {
        ImGui::ShowDemoWindow();
      }

      // TODO: See above.

      timestep = timestep_timer.GetIntervalElapsed();

      scripting::OnLoop(timestep);

      // TODO: See above.

      if (!utility::test_mode)
      {
        ImGui::BeginTabBar("Lists");
      }

      usertypes::SortObjects();
      usertypes::UpdateObjects(timestep);
      usertypes::UpdateGameScenes();
      usertypes::UpdateResources();

      if (!utility::test_mode)
      {
        ImGui::EndTabBar();
      }

      utility::ImGui_EndFrame();

      // TODO: See above.

      usertypes::FlagGameObjectsWithFlaggedGameScenes();
      usertypes::ClearFlaggedObjects();
      usertypes::ClearFlaggedResources();

      // TODO: See above.

      system::NextFrame();

      events::UpdatePrevInputState();

      system::CalculateFPS();
      // TODO: See above.

      system::Delay();
      // TODO: See above.
    }
  }
}