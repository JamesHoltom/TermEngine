#include <string>

#include "Application.h"
#include "../logging/Logger.h"
#include "../events/InputManager.h"
#include "../fonts/FontAtlasManager.h"
#include "../glyphs/GlyphSet.h"
#include "../scripting/ScriptingInterface.h"
#include "../scenes/SceneManager.h"
#include "../shaders/ShaderManager.h"
#include "../system/Window.h"
#include "../timing/FPSManager.h"

namespace term_engine::application {
  void Init()
  {
    scripting::InitScript();

    // Create the default scene, shader and font.
    shaders::InitGlyphShader();
    fonts::AddFontAtlas(fonts::FontAtlasKey(std::string(fonts::DEFAULT_FONT), fonts::DEFAULT_FONT_SIZE));
    scenes::active_scene_ = scenes::AddScene("default");

    logging::logger->debug("Finished init!");
  }

  void CleanUp()
  {
    scripting::CleanUp();
    scenes::CleanUpScenes();
    fonts::CleanUpFontAtlas();
    shaders::CleanUpShaders();

    logging::logger->debug("Cleaned up!");
  }

  void Run()
  {
    bool quit = false;

    scripting::OnInit();

    while (!quit) {
      // Using the side-effect of SDL_QuitRequested() calling SDL_PumpEvents() to omit it from the below code.
      if (SDL_QuitRequested()) {
        if (scripting::OnQuit()) {
          quit = true;
        }
      }

      events::UpdateEvents();

      scripting::OnLoop();

      system::ClearWindow();

      scenes::active_scene_->Render();

      system::RefreshWindow();

      timing::NextFrame();

      events::UpdatePrevEvents();

      timing::CalculateFPS();
      timing::Delay();
    }
  }
}