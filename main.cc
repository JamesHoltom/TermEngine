#include "includes.h"
#include "init.h"
#include "timing/FPSManager.h"
#include "window/Window.h"
#include "window/WindowManager.h"
#include "shaders/ShaderManager.h"
#include "text/GlyphManager.h"
#include "utility/DebugFunctions.h"

#include <string>
#include <spdlog/spdlog.h>

int main(int argc, char** argv) {
  SDL_Event evt;
  bool quit = false;
  int window_size = 0;
  bool wireframe_mode = false;
  int window_num = 0;

  term_engine::InitSDL();
  term_engine::InitGL();

  term_engine::windows::InitDefaultWindow();
  term_engine::shaders::InitGlyphShader();
  term_engine::glyphs::InitExampleGlyphSet();

  term_engine::windows::WindowPtr def_win = term_engine::windows::GetWindow("default");
  def_win->SetShaderProgram("glyph");
  def_win->SetGlyphSet("example");

  term_engine::fps_management::InitFPS();

  while (!quit) {
    while (SDL_PollEvent(&evt) != 0) {
      if (evt.type == SDL_QUIT) {
        quit = true;
      }

      if (evt.type == SDL_KEYUP) {
        switch (evt.key.keysym.sym) {
          case SDLK_q:
            quit = true;

            break;
          case SDLK_f:
            spdlog::info("FPS: {}", term_engine::fps_management::GetAverageFPS());

            break;
          case SDLK_g:
            wireframe_mode = !wireframe_mode;

            if (wireframe_mode) {
              glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            } else {
              glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }

            break;
          case SDLK_w:
            window_size = ++window_size % 3;

            switch (window_size) {
            case 0:
              def_win->SetSize(term_engine::windows::DEFAULT_WINDOW_WIDTH, term_engine::windows::DEFAULT_WINDOW_HEIGHT);

              break;
            case 1:
              def_win->SetSize(320, 240);

              break;
            case 2:
              def_win->SetSize(1024, 800);

              break;
            }

            spdlog::info("Switched to window size {}.", window_size);

            break;
        }
      }

      term_engine::debug::LogWindowEvents(evt);
      term_engine::debug::LogKeyboardEvents(evt);
    }

    term_engine::windows::Logic(evt);
    term_engine::windows::Render();

    term_engine::fps_management::NextFrame();
    term_engine::fps_management::Delay();
  }
  
  term_engine::windows::CleanUpWindows();
  term_engine::shaders::CleanUpShaders();
  term_engine::glyphs::CleanUpGlyphSets();
  term_engine::glyphs::GlyphSet::CleanUpVertices();
  term_engine::Shutdown();

  return 0;
}
