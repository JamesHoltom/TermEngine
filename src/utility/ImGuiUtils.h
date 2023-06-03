/// @author James Holtom

#ifndef IMGUI_UTILS_H
#define IMGUI_UTILS_H

#include <SDL2/SDL.h>
#include <imgui.h>
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_opengl3.h"
#include "misc/freetype/imgui_freetype.h"

namespace term_engine::utility {
  constexpr char IMGUI_GL_VERSION[] = "#version 130";
  constexpr ImVec2 DEBUG_WINDOW_SIZE = ImVec2(240, 480);
  constexpr ImVec2 TEST_WINDOW_SIZE = ImVec2(1080, 800);

  inline bool imgui_open = false;
  inline bool test_mode = false;
  inline ImVec2 imgui_window_size = DEBUG_WINDOW_SIZE;
  inline SDL_Window* imgui_debug_window = nullptr;

  bool InitImGui();

  void PostWindowInitImGui();

  void CleanUpImGui();

  void ImGui_StartFrame();

  void ImGui_EndFrame();

  void ImGui_DoEvents(const SDL_Event& event);
}

#endif // ! IMGUI_UTILS_H