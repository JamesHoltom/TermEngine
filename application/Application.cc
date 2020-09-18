#include "Application.h"

#include <spdlog/spdlog.h>

#include "../init.h"
#include "../timing/FPSManager.h"
#include "../window/Window.h"
#include "../utility/DebugFunctions.h"

namespace term_engine {
  Application::Application():
    event_(SDL_Event()),
    quit_(false)
  {
    if (InitSDL() > 0 || InitGL() > 0 || InitFreeType(&fonts::font_library) > 0) {
      return;
    }

    windows::Init();
    glyphs::InitVertices();
    shaders::InitGlyphShader();
  }

  Application::~Application() {
    glyphs::CleanUpVertices();
    shaders::CleanUpShaders();

    term_engine::windows::CleanUp();
    term_engine::fonts::CleanUpFontAtlas();
    term_engine::CleanUpFreeType(fonts::font_library);
    term_engine::CleanUpSDL();
  }

  void Application::Run() {
    glyphs::GlyphSetPtr glyph_set = glyphs::AddGlyphSet("example", 2, 1);
    fonts::FontAtlasPtr font_atlas = fonts::AddFontAtlas("resources/lucon.ttf", glm::ivec2(32, 32));

    glyphs_ = glyphs::GetGlyphSet("example");
    shader_ = shaders::GetShader("glyph");

    glyphs::GlyphData new_data;
    new_data.instance_offset = glm::vec2(0.25f, 0.0f);
    new_data.instance_scale = glm::vec2(0.5f);
    new_data.foreground_color = glm::vec4(0.5f, 0.5f, 0.0f, 1.0f);
    new_data.background_color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

    glyphs_->SetChar(0, 0, 'A', font_atlas);
    glyphs_->SetData(0, 0, new_data);

    glyphs::GlyphData data = glyphs_->GetData(0, 0);
    spdlog::info("At 0x0:");
    spdlog::info("Offset: {}, {}", data.instance_offset.x, data.instance_offset.y);
    spdlog::info("Scale: {}, {}", data.instance_scale.x, data.instance_scale.y);
    spdlog::info("Texture coords.: {}, {}, {}, {}", data.texture_bounds.x, data.texture_bounds.y, data.texture_bounds.z, data.texture_bounds.w);
    spdlog::info("FG color: {}, {}, {}, {}", data.foreground_color.r, data.foreground_color.g, data.foreground_color.b, data.foreground_color.a);
    spdlog::info("BG color: {}, {}, {}, {}", data.background_color.r, data.background_color.g, data.background_color.b, data.background_color.a);

    while (!quit_) {
      while (SDL_PollEvent(&event_) != 0) {
        if (event_.type == SDL_EventType::SDL_QUIT) {
          quit_ = true;
        }

        debug::LogWindowEvents(event_);
        debug::LogKeyboardEvents(event_);
      }

      glClear(GL_COLOR_BUFFER_BIT);
      glClearColor(0.0f, 0.2f, 0.8f, 1.0f);

      shader_->Use();
      font_atlas->Use();
      glyphs_->Render();
      font_atlas->Unuse();
      shader_->Unuse();

      SDL_GL_SwapWindow(windows::window.get());

      fps_management::NextFrame();
      fps_management::Delay();
    }

    glyph_set.reset();
    font_atlas.reset();
  }
}