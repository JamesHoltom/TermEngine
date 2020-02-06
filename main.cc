#include <functional>
#include <inttypes.h>
#include <stdio.h>

#include "utility/SDLIncludes.h"
#include "utility/CharacterRange.h"
#include "utility/Glyph.h"
#include "modules/CharacterCache.h"
#include "modules/TerminalWindow.h"
#include "modules/Timer.h"

constexpr int WIDTH = 640;
constexpr int HEIGHT = 480;

constexpr int TERM_WIDTH = 32;
constexpr int TERM_HEIGHT = 24;
constexpr int FONT_SIZE = 20;

std::function<term_engine::utilities::Glyph()> gen = [&]() {
  return term_engine::utilities::Glyph(L'\u0021' + (rand() % 94), { 255, 0, 0, 255 }, { 16, 64, 64, 255 });
};

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

  term_engine::modules::CharacterCache char_cache("unifont-12.1.03.ttf", FONT_SIZE);
  term_engine::modules::TerminalWindow term_win(&char_cache);
  term_engine::modules::Timer timer;

  term_win.SetWindowCount(TERM_WIDTH, TERM_HEIGHT);
  term_win.SetWindowSizeByCount();
  term_win.Resize();

  timer.Start();

  bool quit = false;

  while (!quit) {
    uint64_t elapsed = timer.GetIntervalElapsed();
    
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
      term_win.Update(elapsed);

      term_win.SetGlyphs(gen);
      
      SDL_RenderClear(renderer);

      if (term_win.Render(window, renderer) == -1) {
        printf("Failed to render!");
      }

      SDL_RenderPresent(renderer);
    }
  }

  TTF_Quit();
  SDL_Quit();

  return 0;
}
