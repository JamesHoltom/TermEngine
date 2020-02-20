#include <functional>
#include <inttypes.h>
#include <stdio.h>
#include <vector>

#include "utility/SDLIncludes.h"
#include "utility/CharacterRange.h"
#include "utility/Glyph.h"
#include "modules/CharacterCache.h"
#include "modules/EventManager.h"
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

  term_engine::modules::EventManager event;
  term_engine::modules::FPSManager fps;
  term_engine::modules::CharacterCache char_cache("unifont-12.1.03.ttf", FONT_SIZE);
  term_engine::modules::TerminalWindow term_win(&char_cache);
  term_engine::modules::Timer timer;

  /*
   * Predicates that set multiple (not all) glyphs (i.e. using SetGlyphs).
   */
  constexpr int FPS_STR_WIDTH = 10;
  bool showFpsCounter = false;
  std::function<int(std::vector<term_engine::utilities::Glyph>&)> fpsFunc = [&fps, FPS_STR_WIDTH](std::vector<term_engine::utilities::Glyph>& glyphs) {
    float val = fps.GetAverageFPS();
    std::wstring fpsCount = std::wstring(std::to_wstring(val));
    fpsCount.resize(FPS_STR_WIDTH, L'\u0000');
    int counter = 0;

    std::vector<term_engine::utilities::Glyph>::iterator it = glyphs.begin() + (TERM_WIDTH - FPS_STR_WIDTH - 1);

    std::for_each(it, it + FPS_STR_WIDTH, [&fpsCount, &counter](term_engine::utilities::Glyph& n) {
      n.character = fpsCount.at(counter++);
      n.background = { 0, 0, 0, 255 };
      n.foreground = { 255, 255, 255, 255 };
    });

    return 0;
  };

  float player_x = 0.0f;
  float player_y = 0.0f;
  int prevIndex = -1;
  term_engine::utilities::Glyph prevGlyph = term_engine::utilities::nullGlyph;
  std::function<int(std::vector<term_engine::utilities::Glyph>&)> playerFunc = [&player_x, &player_y, &prevIndex, &prevGlyph](std::vector<term_engine::utilities::Glyph>& glyphs) {
    term_engine::utilities::Glyph glyph;
    glyph.character = L'\u2603';
    glyph.background = { 0, 0, 0, 255 };
    glyph.foreground = { 0, 255, 0, 255 };

    int index = (TERM_WIDTH * (int)player_y) + (int)player_x;

    if (index != prevIndex) {
      if (prevIndex != -1) {
        glyphs.at(prevIndex) = prevGlyph;
      }

      prevIndex = index;
      prevGlyph = glyphs.at(index);

      glyphs.at(index) = glyph;
    }
    
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
  
  event.RegisterAction("test");
  event.RegisterKey(SDLK_k);

  bool quit = false;
  uint64_t elapsed = 0;

  bool move_up = false;
  bool move_down = false;
  bool move_left = false;
  bool move_right = false;

  while (!quit) {
    elapsed = timer.GetIntervalElapsed();
    startTime = timer.GetDuration();
    
    float speed = 25.0f;
    float rate = (float)elapsed / 1000.0f;

    while (SDL_PollEvent(&evt) != 0) {
      if (evt.type == SDL_QUIT) {
        quit = true;
      }
      
      if (evt.type == SDL_KEYDOWN) {
        switch (evt.key.keysym.sym) {
        case SDLK_UP:
          move_up = true;

          break;
        case SDLK_DOWN:
          move_down = true;

          break;
        case SDLK_LEFT:
          move_left = true;

          break;
        case SDLK_RIGHT:
          move_right = true;

          break;
        }
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
          term_win.ClearGlyphs();
          renderVer = (renderVer + 1) % 4;
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
        case SDLK_g:
          showFpsCounter = !showFpsCounter;
          
          break;
        case SDLK_UP:
          move_up = false;

          break;
        case SDLK_DOWN:
          move_down = false;

          break;
        case SDLK_LEFT:
          move_left = false;

          break;
        case SDLK_RIGHT:
          move_right = false;

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
      case 3:
        if (move_up) {
          player_y -= speed * rate;

          if (player_y < 0.0f) {
            player_y = 0.0f;
          }
        }

        if (move_down) {
          player_y += speed * rate;

          if (player_y > (float)(TERM_HEIGHT - 1)) {
            player_y = (float)(TERM_HEIGHT - 1);
          }
        }

        if (move_left) {
          player_x -= speed * rate;

          if (player_x < 0.0f) {
            player_x = 0.0f;
          }
        }

        if (move_right) {
          player_x += speed * rate;

          if (player_x > (float)(TERM_WIDTH - 1)) {
            player_x = (float)(TERM_WIDTH - 1);
          }
        }

        term_win.SetGlyphs(playerFunc);
        break;
      }
      
      if (showFpsCounter) {
        term_win.SetGlyphs(fpsFunc);
      }
      
      SDL_RenderClear(renderer);

      if (term_win.Render(window, renderer) == -1) {
        printf("Failed to render!");
      }

      SDL_RenderPresent(renderer);
    }

    fps.NextFrame();

    if (fps.isUsingTargetFPS()) {
      fps.Delay();
    }
  }
  
  event.UnregisterAction("test");
  event.UnregisterKey(SDLK_k);

  TTF_Quit();
  SDL_Quit();

  return 0;
}
