#include "includes.h"
#include "init.h"
#include "timing/FPSManager.h"
#include "rendering/VertexManager.h"
#include "rendering/Window.h"
#include "rendering/WindowManager.h"
#include "rendering/ShaderManager.h"
#include "utility/DebugFunctions.h"

#include <string>
#include <spdlog/spdlog.h>

int main(int argc, char** argv) {
  SDL_Event evt;
  bool quit = false;
  int window_num = 0;

  term_engine::InitSDL();
  term_engine::InitGL();

  term_engine::window_management::InitDefaultWindow();

  term_engine::CreateVertexData();

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
          case SDLK_i:
            std::string window_str = "test_" + std::to_string(window_num++);
            term_engine::window_management::AddWindow(window_str);
            term_engine::shaders::BuildResult frag_result = term_engine::shaders::BuildShaderFromString(term_engine::shaders::default_fragment_shader, GL_FRAGMENT_SHADER);
            term_engine::shaders::BuildResult vert_result = term_engine::shaders::BuildShaderFromString(term_engine::shaders::default_vertex_shader, GL_VERTEX_SHADER);

            if (std::get<0>(vert_result) == GL_TRUE && std::get<0>(frag_result) == GL_TRUE) {
              term_engine::shaders::BuildResult prog_result = term_engine::shaders::BuildProgram(window_str, { std::get<1>(vert_result), std::get<1>(frag_result) });

              if (std::get<0>(prog_result) == GL_TRUE) {
                term_engine::window_management::GetWindow(window_str)->SetProgram(std::get<1>(prog_result));
              }
            }

            spdlog::info("Created window {}.", window_str);

            break;
        }
      }

      term_engine::debug::LogWindowEvents(evt);
      term_engine::debug::LogKeyboardEvents(evt);
      term_engine::window_management::Logic(evt);
    }

    term_engine::window_management::Render();

    term_engine::fps_management::NextFrame();
    term_engine::fps_management::Delay();
  }
  
  term_engine::window_management::CleanUpWindows();
  term_engine::Shutdown();

  return 0;
}
