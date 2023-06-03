#include "../events/InputManager.h"
#include "ImGuiUtils.h"
#include "GLUtils.h"
#include "SpdlogUtils.h"

namespace term_engine::utility {
  bool InitImGui()
  {
    IMGUI_CHECKVERSION();

    imgui_debug_window = SDL_CreateWindow("TermEngine Debug Window", 0, 0, (int)DEBUG_WINDOW_SIZE.x, (int)DEBUG_WINDOW_SIZE.y, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN);

    if (imgui_debug_window == nullptr)
    {
      logger->error("Failed to create debug window: {}", SDL_GetError());

      return false;
    }

    return true;
  }

  void PostWindowInitImGui()
  {
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    SDL_SetWindowSize(imgui_debug_window, (int)DEBUG_WINDOW_SIZE.x, (int)DEBUG_WINDOW_SIZE.y);

    ImGui_ImplSDL2_InitForOpenGL(imgui_debug_window, context);
    ImGui_ImplOpenGL3_Init(IMGUI_GL_VERSION);
  }

  void CleanUpImGui()
  {
    if (is_context_created)
    {
      ImGui_ImplOpenGL3_Shutdown();
      ImGui_ImplSDL2_Shutdown();
    }

    ImGui::DestroyContext();

    SDL_DestroyWindow(imgui_debug_window);
  }

  void ImGui_StartFrame()
  {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();

    ImGui::NewFrame();
    ImGui::Begin("TermEngine - Debug Window", &utility::imgui_open, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);

    ImGui::SetWindowPos(ImVec2(0.0f, 0.0f));

    if (test_mode)
    {
      ImGui::SetWindowSize(TEST_WINDOW_SIZE);
    }
    else
    {
      ImGui::SetWindowSize(imgui_window_size);
    }
  }

  void ImGui_EndFrame()
  {
    ImGui::End();

    SDL_GL_MakeCurrent(imgui_debug_window, context);
    SDL_GL_SetSwapInterval(0);

    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = nullptr;

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glViewport(0, 0, (int)imgui_window_size.x, (int)imgui_window_size.y);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    SDL_GL_SwapWindow(imgui_debug_window);
  }

  void ImGui_DoEvents(const SDL_Event& event)
  {
    if (events::KeyIsPressed("F3"))
    {
      if (!imgui_open)
      {
        SDL_ShowWindow(imgui_debug_window);
        imgui_open = true;
      }

      test_mode = false;

      SDL_SetWindowSize(imgui_debug_window, (int)imgui_window_size.x, (int)imgui_window_size.y);
      SDL_SetWindowResizable(imgui_debug_window, SDL_TRUE);
    }

    if (events::KeyIsPressed("F4"))
    {
      if (!imgui_open)
      {
        SDL_ShowWindow(imgui_debug_window);
        imgui_open = true;
      }

      test_mode = true;

      SDL_SetWindowSize(imgui_debug_window, (int)TEST_WINDOW_SIZE.x, (int)TEST_WINDOW_SIZE.y);
      SDL_SetWindowResizable(imgui_debug_window, SDL_FALSE);
    }

    if (event.type == SDL_WINDOWEVENT && event.window.windowID == SDL_GetWindowID(imgui_debug_window))
    {
      if (event.window.event == SDL_WINDOWEVENT_RESIZED)
      {
        imgui_window_size = ImVec2(event.window.data1, event.window.data2);
      }
      else if (event.window.event == SDL_WINDOWEVENT_CLOSE)
      {
        SDL_HideWindow(imgui_debug_window);
        imgui_open = false;
      }
    }
  }
}
