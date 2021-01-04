#include <string>

#include "Application.h"
#include "../logging/Logger.h"
#include "../utility/init.h"
#include "../utility/SDLUtils.h"
#include "../fonts/FontAtlas.h"
#include "../glyphs/GlyphSet.h"
#include "../events/InputManager.h"
#include "../fonts/FontAtlasManager.h"
#include "../scenes/SceneManager.h"
#include "../scripting/ScriptingInterface.h"
#include "../shaders/ShaderManager.h"
#include "../timing/FPSManager.h"
#include "../window/Window.h"

namespace term_engine::application {
  void Init()
  {
    logging::InitLogger();

    if (InitSDL() > 0 || InitGL() > 0 || InitFreeType(&fonts::font_library) > 0) {
      logging::logger->critical("Failed to initialise SDL/GL/FT!");

      return;
    }

    windows::Init();
    events::Init();
    glyphs::InitVertexData();
    shaders::InitGlyphShader();
    fonts::AddFontAtlas(std::string(fonts::DEFAULT_FONT), fonts::DEFAULT_FONT_SIZE);
    scenes::active_scene_ = scenes::AddScene("default");

    scripting::InitInterface();
    scripting::InitScript();

    timing::InitFPS();
  }

  void CleanUp()
  {
    events::CleanUp();
    scripting::CleanUp();
    scenes::CleanUpScenes();
    glyphs::CleanUpVertexData();
    fonts::CleanUpFontAtlas();
    shaders::CleanUpShaders();
    windows::CleanUp();

    CleanUpFreeType(fonts::font_library);
    CleanUpSDL();
  }

  void Run()
  {
    bool quit = false;
    glyphs::GlyphSetPtr glyph_set = scenes::active_scene_->GetGlyphSet();

    glyph_set->ResetProjection();

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

      glClear(GL_COLOR_BUFFER_BIT);
      glClearColor(0.0f, 0.2f, 0.5f, 1.0f);

      scenes::active_scene_->Render();

      SDL_GL_SwapWindow(windows::window.get());

      timing::NextFrame();

      events::UpdatePrevEvents();

      timing::CalculateFPS();
      timing::Delay();
    }
  }
}