#include <functional>
#include <inttypes.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "modules/CharacterCache.h"
#include "modules/TerminalWindow.h"
#include "modules/Timer.h"
#include "utility/CharacterRange.h"
#include "utility/Glyph.h"

constexpr int WIDTH = 640;
constexpr int HEIGHT = 480;

constexpr int TERM_WIDTH = 32;
constexpr int TERM_HEIGHT = 24;
constexpr int FONT_SIZE = 20;

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

      /*for (int j = 0; j < TERM_HEIGHT - 1; j++) {
        for (int i = 0; i < TERM_WIDTH; i++) {
          term_engine::utilities::Glyph tmp = {
            L'\u0021',
            { 255, 0, 0, 255 },
            { 255, 255, 255, 255 }
          };
          
          if (term_win.SetGlyph(i, j, tmp) == -1) {
            printf("Failed to set glyph for [%i,%i]!", i, j);
          }
        }
      }*/
      
      std::function<term_engine::utilities::Glyph()> gen = []() {
        return (term_engine::utilities::Glyph){
          L'\u0021',
          { 255, 0, 0, 255 },
          { 255, 255, 255, 255 }
        };
      };
      
      term_win.SetGlyphs(gen);
      
      SDL_RenderClear(renderer);

      SDL_Surface* srf = SDL_CreateRGBSurfaceWithFormat(0, 256, 256, 32, SDL_PIXELFORMAT_RGBA32);
      SDL_Rect fill = { 0, 0, 256, 256 };
      SDL_Rect set = { WIDTH - 256, HEIGHT - 256, 256, 256 };
      SDL_FillRect(srf, &fill, SDL_MapRGBA(srf->format, 255, 0, 0, 255));
      SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, srf);
      SDL_RenderCopy(renderer, tex, nullptr, &set);
      SDL_DestroyTexture(tex);
      SDL_FreeSurface(srf);
      
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
