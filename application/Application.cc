#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Application.h"
#include "../init.h"
#include "../timing/FPSManager.h"
#include "../window/Window.h"
#include "../utility/DebugFunctions.h"
#include "../utility/spdlogUtils.h"

namespace term_engine {
  Application::Application():
    event_(SDL_Event()),
    quit_(false)
  {
    spdlog::InitSpdLog();

    if (InitSDL() > 0 || InitGL() > 0 || InitFreeType(&fonts::font_library) > 0) {
      return;
    }

    windows::Init();
    shaders::InitGlyphShader();

    glyphs::InitVertexData();
  }

  Application::~Application() {
    glyphs::CleanUpGlyphSets();
    glyphs::CleanUpVertexData();
    shaders::CleanUpShaders();
    windows::CleanUp();
    fonts::CleanUpFontAtlas();

    CleanUpFreeType(fonts::font_library);
    CleanUpSDL();
  }

  void Application::Run() {
    const int set_width = 20, set_height = 10;
    int set_pattern = 0, set_timer = 0;
    int calc = 0;

    int window_width, window_height;
    SDL_GetWindowSize(windows::window.get(), &window_width, &window_height);

    glyphs::GlyphSetPtr glyph_set = glyphs::AddGlyphSet("example", set_width, set_height);
    fonts::FontAtlasPtr font_atlas = fonts::AddFontAtlas("resources/lucon.ttf", 28);
    glm::ivec2 tex_size = font_atlas->GetTextureSize();
    int texture_id = 0;

    glm::mat4 projection = glm::ortho(0.0f, (GLfloat)window_width, (GLfloat)window_height, 0.0f);

    glyph_set->SetScale(tex_size.x, tex_size.y);
    glyph_set->SetSpacing(5, 15);
    glyph_set->ResetAllOffsets();

    shader_ = shaders::GetShader("glyph");
    shader_->Use();
    shader_->SetUniformMatrix("projection", glm::ivec2(4, 4), glm::value_ptr(projection));
    shader_->SetUniformFloat("scale", 2, glm::value_ptr(glyph_set->GetScale()));
    shader_->SetUniformInt("font_texture", 1, &texture_id);

    timing::InitFPS();

    script_.RunInit();
    script_.RunTest();

    while (!quit_) {
      while (SDL_PollEvent(&event_) != 0) {
        if (event_.type == SDL_EventType::SDL_QUIT) {
          quit_ = true;
        }

        if (event_.type == SDL_EventType::SDL_KEYDOWN) {
          switch (event_.key.keysym.sym) {
          case SDL_KeyCode::SDLK_f:
            timing::PrintFPS();

            break;
          case SDL_KeyCode::SDLK_g:
            set_pattern = (set_pattern + 1) % 2;
          }
        }

        debug::LogWindowEvents(event_);
        debug::LogKeyboardEvents(event_);
      }

      set_timer = (set_timer + 1) % 360;
      calc = 30 + (int)(sin(set_timer * 3.1415f / 180.0f) * 30);
      glyph_set->SetPosition(calc, 20);

      ChangeSet(glyph_set, font_atlas, set_pattern);

      glClear(GL_COLOR_BUFFER_BIT);
      glClearColor(0.0f, 0.2f, 0.5f, 1.0f);

      font_atlas->Use();
      glyph_set->Render();
      font_atlas->Unuse();

      SDL_GL_SwapWindow(windows::window.get());

      timing::NextFrame();
      timing::CalculateFPS();
      timing::Delay();
    }

    shader_->Unuse();

    glyph_set.reset();
    font_atlas.reset();
  }

  void ChangeSet(glyphs::GlyphSetPtr& set, fonts::FontAtlasPtr& atlas, const int& pattern) {
    int count = 0;
    glm::ivec2 set_size = set->GetSize();
    while (count < set->GetCount()) {
      int x = count % set_size.x;
      int y = count / set_size.x;
      char str;
      glm::vec4 fg_color;
      glm::vec4 bg_color;

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

      set->SetChar(x, y, str, atlas);
      set->SetFGColor(x, y, fg_color);
      set->SetBGColor(x, y, bg_color);

      count++;
    }
  }
}