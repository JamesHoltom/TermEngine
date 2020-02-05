#include <inttypes.h>
#include <stdio.h>
#include <SDL.h>
#include <SDL_ttf.h>

#include "modules/CharacterCache.h"
#include "modules/TerminalWindow.h"
#include "modules/Timer.h"
#include "utility/CharacterRange.h"
#include "utility/Glyph.h"

constexpr int WIDTH = 640;
constexpr int HEIGHT = 480;

int main(int argc, char** argv) {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    return -1;
  }

  if (TTF_Init() < 0) {
    return -1;
  }

  SDL_Window* window = SDL_CreateWindow("TermEngine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_OPENGL);
  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  
  SDL_Event evt;

  term_engine::utilities::InitCharacterRanges();

  constexpr int TERM_WIDTH = 32;
  constexpr int TERM_HEIGHT = 24;
  constexpr int FONT_SIZE = 20;

  term_engine::modules::CharacterCache char_cache("unifont-12.1.03.ttf", FONT_SIZE);
  term_engine::modules::TerminalWindow term_win(&char_cache);
  term_engine::modules::Timer timer;
  term_engine::modules::Timer int_timer;

  term_win.SetWindowCount(TERM_WIDTH, TERM_HEIGHT);
  term_win.SetWindowSizeByCount();

  timer.Start();
  int_timer.Start();

  srand(SDL_GetTicks());

  bool quit = false;

  while (!quit) {
    uint64_t lap_a;
    uint64_t lap_b;
    uint64_t lap_c;

    while (SDL_PollEvent(&evt) != 0) {
      if (evt.type == SDL_QUIT) {
        quit = true;
      }

      if (evt.type == SDL_KEYUP && evt.key.keysym.sym == SDLK_p) {
        if (timer.IsPaused()) {
          timer.Resume();
        }
        else {
          timer.Pause();
        }
      }
    }

    if (!timer.IsPaused()) {
      uint64_t elapsed = timer.GetIntervalElapsed();
      lap_a = int_timer.GetIntervalElapsed();

      term_win.Update(elapsed);
      lap_b = int_timer.GetIntervalElapsed();

      for (int j = 0; j < TERM_HEIGHT; ++j) {
        for (int i = 0; i < TERM_WIDTH; ++i) {
          Uint8 r = (Uint8)(rand()) % 256;
          Uint8 g = (Uint8)(rand()) % 256;
          Uint8 b = (Uint8)(rand()) % 256;

          term_engine::utilities::Glyph tmp = {
            L'\u0021' + (rand() % 94),
            { r, g, b, 255 },
            { 255 - r, 255 - g, 255 - b, 255 }
          };

          term_win.SetGlyph(i, j, tmp);
        }
      }
      lap_c = int_timer.GetIntervalElapsed();

      SDL_RenderClear(renderer);

      term_win.Render(window, renderer);

      SDL_RenderPresent(renderer);

      printf_s("Loop Start: %" PRId64 "\nEvents: %" PRId64 "\nUpdate: %" PRId64 "\nRender: %" PRId64 "\n", lap_a, lap_b, lap_c, int_timer.GetIntervalElapsed());
    }
  }

  TTF_Quit();
  SDL_Quit();

  return 0;
}
