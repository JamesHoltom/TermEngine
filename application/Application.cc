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
    glyphs::GlyphSetPtr glyph_set = glyphs::AddGlyphSet("example", 4, 1);
    fonts::FontAtlasPtr font_atlas = fonts::AddFontAtlas("resources/lucon.ttf", glm::ivec2(128));
    int texture_id = 0;

    int window_width, window_height;
    SDL_GetWindowSize(windows::window.get(), &window_width, &window_height);

    glm::mat4 projection = glm::ortho(0.0f, (GLfloat)window_width, (GLfloat)window_height, 0.0f);

    glyph_set->SetScale(100, 100);

    shader_ = shaders::GetShader("glyph");
    shader_->Use();
    shader_->SetUniformMatrix("projection", glm::ivec2(4, 4), glm::value_ptr(projection));
    shader_->SetUniformInt("scale", 2, glm::value_ptr(glyph_set->GetScale()));
    shader_->SetUniformInt("font_texture", 1, &texture_id);

    // Add an 'A' character.
    glyph_set->SetChar(0, 0, 'A', font_atlas);
    glyph_set->SetOffset(0, 0, glm::vec2(0.0f, 0.0f));
    glyph_set->SetFGColor(0, 0, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
    glyph_set->SetBGColor(0, 0, glm::vec4(0.0f, 1.0f, 1.0f, 1.0f));

    // Add an 'B' character.
    glyph_set->SetChar(1, 0, 'B', font_atlas);
    glyph_set->SetOffset(1, 0, glm::vec2(120.0f, 50.0f));
    glyph_set->SetFGColor(1, 0, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
    glyph_set->SetBGColor(1, 0, glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));

    // Add an 'C' character.
    glyph_set->SetChar(2, 0, 'C', font_atlas);
    glyph_set->SetOffset(2, 0, glm::vec2(230.0f, 50.0f));
    glyph_set->SetFGColor(2, 0, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
    glyph_set->SetBGColor(2, 0, glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));

    // Add an 'D' character.
    glyph_set->SetChar(3, 0, 'D', font_atlas);
    glyph_set->SetOffset(3, 0, glm::vec2(340.0f, 50.0f));
    glyph_set->SetFGColor(3, 0, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    glyph_set->SetBGColor(3, 0, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

    timing::InitFPS();

    while (!quit_) {
      while (SDL_PollEvent(&event_) != 0) {
        if (event_.type == SDL_EventType::SDL_QUIT) {
          quit_ = true;
        }

        debug::LogWindowEvents(event_);
        debug::LogKeyboardEvents(event_);
      }

      glClear(GL_COLOR_BUFFER_BIT);
      glClearColor(0.0f, 0.2f, 0.5f, 1.0f);

      font_atlas->Use();
      glyph_set->Render();
      font_atlas->Unuse();

      SDL_GL_SwapWindow(windows::window.get());

      timing::NextFrame();
      timing::Delay();
    }

    shader_->Unuse();

    glyph_set.reset();
    font_atlas.reset();
  }
}