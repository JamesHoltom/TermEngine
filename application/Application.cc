#include "Application.h"
#include "../init.h"
#include "../fonts/FontAtlasManager.h"
#include "../scenes/SceneManager.h"
#include "../shaders/ShaderManager.h"
#include "../timing/FPSManager.h"
#include "../window/Window.h"
#include "../utility/DebugFunctions.h"

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

namespace term_engine::application {
  void Init() {
    logging::InitLogger();

    if (InitSDL() > 0 || InitGL() > 0 || InitFreeType(&fonts::font_library) > 0) {
      return;
    }

    windows::Init();
    glyphs::InitVertexData();
    shaders::InitGlyphShader();
    fonts::AddFontAtlas(std::string(fonts::DEFAULT_FONT), fonts::DEFAULT_FONT_SIZE);
    fonts::AddFontAtlas("resources/fonts/arial.ttf", fonts::DEFAULT_FONT_SIZE);
    scenes::active_scene_ = scenes::AddScene("default");

    scripting::InitInterface();
    timing::InitFPS();
  }

  void CleanUp() {
    scenes::CleanUpScenes();
    glyphs::CleanUpVertexData();
    fonts::CleanUpFontAtlas();
    shaders::CleanUpShaders();
    windows::CleanUp();

    CleanUpFreeType(fonts::font_library);
    CleanUpSDL();
  }

  void Run() {
    SDL_Event event_;
    bool quit_ = false;
    int set_pattern = 0, font_pattern = 0;
    glyphs::GlyphSetPtr glyph_set = scenes::active_scene_->GetGlyphSet();

    glyph_set->ResetProjection();

    scripting::Boot();
    ChangeSet(set_pattern);

    while (!quit_) {
      while (SDL_PollEvent(&event_) != 0) {
        if (event_.type == SDL_EventType::SDL_QUIT) {
          quit_ = true;
        }

        if (event_.type == SDL_EventType::SDL_KEYDOWN) {
          switch (event_.key.keysym.sym) {
          case SDL_KeyCode::SDLK_f:
            font_pattern = (font_pattern + 1) % 2;

            ChangeFont(font_pattern);
            
            break;
          case SDL_KeyCode::SDLK_g:
            set_pattern = (set_pattern + 1) % 2;

            ChangeSet(set_pattern);

            break;
          case SDL_KeyCode::SDLK_p:
            if ((event_.key.keysym.mod & KMOD_SHIFT) != 0) {
              glyph_set->PrintData();
            }
            else {
              timing::PrintFPS();
            }

            break;
          case SDL_KeyCode::SDLK_s:
            scripting::Run("");

            break;
          case SDL_KeyCode::SDLK_t:
            logging::logger->info("There are {} refs.", glyph_set.use_count());

            break;
          }
        }

        debug::LogWindowEvents(event_);
        debug::LogKeyboardEvents(event_);
      }

      glClear(GL_COLOR_BUFFER_BIT);
      glClearColor(0.0f, 0.2f, 0.5f, 1.0f);

      scenes::active_scene_->Render();

      SDL_GL_SwapWindow(windows::window.get());

      timing::NextFrame();
      timing::CalculateFPS();
      timing::Delay();
    }
  }

  void ChangeFont(const int& pattern) {
    glyphs::GlyphSetPtr glyph_set = scenes::active_scene_->GetGlyphSet();
    fonts::FontAtlasPtr font_atlas;

    switch (pattern) {
    case 0:
      font_atlas = fonts::GetFontAtlas(fonts::DEFAULT_FONT);

      break;
    case 1:
      font_atlas = fonts::GetFontAtlas("resources/fonts/arial.ttf");

      break;
    }

    glyph_set->SetFont(font_atlas);
    glyph_set->ResetScale();
    
    font_atlas.reset();

    logging::logger->info("Switched font.");
  }

  void ChangeSet(const int& pattern) {
    int count = 0;
    glyphs::GlyphSetPtr glyph_set = scenes::active_scene_->GetGlyphSet();

    glm::ivec2 set_size = glyph_set->GetSize();
    while (count < glyph_set->GetCount()) {
      unsigned int x = count % set_size.x;
      unsigned int y = count / set_size.x;
      char str = ' ';
      glm::vec4 fg_color = glyphs::DEFAULT_FOREGROUND_COLOR;
      glm::vec4 bg_color = glyphs::DEFAULT_BACKGROUND_COLOR;

      glyphs::GlyphData& glyph = glyph_set->GetGlyph(glm::uvec2(x, y));

      switch (pattern) {
      case 0:
        str = (char)('a' + (rand() % 26));
        fg_color = glm::vec4((x + y + 1) % 2, (x + y + 1) % 2, (x + y + 1) % 2, 1.0f);
        bg_color = glm::vec4((x + y) % 2, (x + y) % 2, (x + y) % 2, 1.0f);

        break;
      case 1:
        str = (char)('a' + (count % 26));
        fg_color = glm::vec4(1.0f);
        bg_color = glm::vec4((float)x / set_size.x, 0.0f, (float)y / set_size.y, 1.0f);

        break;
      }

      glyph.character_ = str;
      glyph.foreground_color_ = fg_color;
      glyph.background_color_ = bg_color;

      count++;
    }

    glyph_set->Dirty();

    logging::logger->info("Switched pattern.");
  }
}