#include <functional>
#include <inttypes.h>
#include <stdio.h>
#include <vector>

#include "utility/SDLIncludes.h"
#include "utility/CharacterRange.h"
#include "utility/Glyph.h"
#include "modules/CharacterCache.h"
#include "modules/FPSManager.h"
#include "modules/TerminalWindow.h"
#include "modules/Timer.h"

constexpr int WIDTH = 640;
constexpr int HEIGHT = 480;

constexpr int TERM_WIDTH = 32;
constexpr int TERM_HEIGHT = 24;
constexpr int FONT_SIZE = 20;

constexpr double DEG_TO_RAD = M_PI / 180.0f;

int main(int argc, char** argv) {
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    return -1;
  }

  if (TTF_Init() < 0) {
    return -1;
  }

  srand(SDL_GetTicks());

  SDL_Window* window = SDL_CreateWindow("TermEngine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_OPENGL);
  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  
  SDL_Event evt;

  term_engine::utilities::InitCharacterRanges();

  term_engine::modules::FPSManager fps;
  term_engine::modules::CharacterCache char_cache("unifont-12.1.03.ttf", FONT_SIZE);
  term_engine::modules::TerminalWindow term_win(&char_cache);
  term_engine::modules::Timer timer;

  /*
   * Predicates that set multiple (not all) glyphs (i.e. using SetGlyphs).
   */
  uint64_t elapsed = 0;
  float player_x = 0.0f;
  float player_y = 0.0f;
  std::function<int(std::vector<term_engine::utilities::Glyph>&)> playerGen = [&timer, &player_x, &player_y](std::vector<term_engine::utilities::Glyph>& glyphs) {
    
    
    return 0;
  };

  /*
   * Predicates that set all glyphs (i.e. using FillGlyphs).
   */
  int waveIndex = 0;
  uint64_t startTime = 0;
  int renderVer = 0;
  std::function<term_engine::utilities::Glyph()> waveGen = [&waveIndex, &startTime]() {
    term_engine::utilities::Glyph glyph;

    glyph.character = L'\u0021';

    int duration = 2500;
    uint64_t counter = startTime % duration;
    int x_pos = waveIndex % TERM_WIDTH;
    int y_pos = waveIndex / TERM_WIDTH;

    double val_fg = round(sin((double)counter / duration * 360.0f * DEG_TO_RAD + ((x_pos + y_pos) * 0.2f)) * 127.5f) + 127.5f;
    double val_bg = round(cos((double)counter / duration * 360.0f * DEG_TO_RAD + ((x_pos + y_pos) * 0.2f)) * 127.5f) + 127.5f;

    glyph.foreground = { 255, (Uint8)(val_bg), (Uint8)(val_bg), 255 };
    glyph.background = { (Uint8)val_fg, 0, 0, 255 };

    waveIndex++;

    return glyph;
  };

  std::function<term_engine::utilities::Glyph()> randTextGen = [&]() {
    return term_engine::utilities::Glyph(L'\u0021' + (rand() % 94), { 255, 255, 255, 255 }, { 64, 64, 64, 255 });
  };

  std::function<term_engine::utilities::Glyph()> randColorGen = [&]() {
    Uint8 rnd_r = rand() % 255;
    Uint8 rnd_g = rand() % 255;
    Uint8 rnd_b = rand() % 255;

    return term_engine::utilities::Glyph(L'\u0021', { rnd_r, rnd_g, rnd_b, 255 }, { (Uint8)(255 - rnd_r), (Uint8)(255 - rnd_g), (Uint8)(255 - rnd_b), 255 });
  };

  term_win.SetWindowCount(TERM_WIDTH, TERM_HEIGHT);
  term_win.SetWindowSizeByCount();
  term_win.Resize();

  timer.Start();

  bool quit = false;

  while (!quit) {
    elapsed = timer.GetIntervalElapsed();
    startTime = timer.GetDuration();
    
    while (SDL_PollEvent(&evt) != 0) {
      if (evt.type == SDL_QUIT) {
        quit = true;
      }
      
      if (evt.type == SDL_KEYUP) {
        switch (evt.key.keysym.sym) {
        case SDLK_p:
          if (timer.IsPaused()) {
            timer.Resume();
          }
          else {
            timer.Pause();
          }

          break;
        case SDLK_o:
          renderVer = (renderVer + 1) % 3;
          printf("Switching to mode %i.\n", renderVer);

          break;
        case SDLK_f:
          switch (fps.GetTargetFPS()) {
          case 60:
            fps.SetTargetFPS(30);
            break;
          case 30:
            fps.SetTargetFPS(20);
            break;
          case 20:
            fps.SetTargetFPS(15);
            break;
          case 15:
            fps.SetTargetFPS(5);
            break;
          case 5:
            fps.DisableTargetFPS();
            break;
          case 0:
            fps.SetTargetFPS(60);
            break;
          }

          if (fps.isUsingTargetFPS()) {
            printf("Set FPS cap to %i\n", fps.GetTargetFPS());
          }
          else {
            printf("Disabled FPS cap.\n");
          }

          break;
        }
      }
    }

    if (!timer.IsPaused()) {
      term_win.Update(elapsed);

      switch (renderVer) {
      case 0:
        term_win.FillGlyphs(randTextGen);
        break;
      case 1:
        term_win.FillGlyphs(randColorGen);
        break;
      case 2:
        waveIndex = 0;
        term_win.FillGlyphs(waveGen);
        break;
      }
      
      SDL_RenderClear(renderer);

      if (term_win.Render(window, renderer) == -1) {
        printf("Failed to render!");
      }

      SDL_RenderPresent(renderer);

      if (fps.isUsingTargetFPS()) {
        fps.DelayUntilInterval();
      }
    }
  }

  TTF_Quit();
  SDL_Quit();

  return 0;
}
