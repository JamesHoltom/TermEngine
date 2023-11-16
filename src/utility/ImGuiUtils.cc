#include "../events/InputManager.h"
#include "../system/CLArguments.h"
#include "../system/FileFunctions.h"
#include "../system/FPSManager.h"
#include "../usertypes/EventListener.h"
#include "../usertypes/GameScene.h"
#include "../usertypes/game_objects/BaseObject.h"
#include "../usertypes/resources/BaseResource.h"
#include "../usertypes/resources/Font.h"
#include "ImGuiUtils.h"
#include "GLUtils.h"
#include "SpdlogUtils.h"

namespace term_engine::utility {
  bool InitImGui()
  {
    if (!system::debug_mode)
    {
      return true;
    }

    uint32_t flags = SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN | SDL_WINDOW_RESIZABLE;

    IMGUI_CHECKVERSION();

    imgui_debug_window = SDL_CreateWindow("TermEngine Debug Window", 0, 0, (int)DEBUG_WINDOW_SIZE.x, (int)DEBUG_WINDOW_SIZE.y, flags);

    if (imgui_debug_window != nullptr)
    {
      logger->debug("Created debug window with ID {}.", SDL_GetWindowID(imgui_debug_window));
    }
    else
    {
      logger->error("Failed to create debug window: {}", SDL_GetError());

      return false;
    }

    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = nullptr;

    const std::filesystem::path fontPath = system::SearchForResourcePath(std::string(usertypes::DEFAULT_FONT));

    if (fontPath != "")
    {
      io.Fonts->AddFontFromFileTTF(fontPath.c_str(), 16.0f, nullptr, io.Fonts->GetGlyphRangesCyrillic());
      io.Fonts->Build();
    }
    else
    {
      logger->error("Failed to generate font for debug window!");

      return false;
    }

    ImGui_ImplSDL2_InitForOpenGL(imgui_debug_window, context);
    ImGui_ImplOpenGL3_Init(IMGUI_GL_VERSION);

    return true;
  }

  void CleanUpImGui()
  {
    if (system::debug_mode)
    {
      ImGui_ImplOpenGL3_Shutdown();
      ImGui_ImplSDL2_Shutdown();

      ImGui::DestroyContext();
    }
    
    if (imgui_debug_window != nullptr)
    {
      SDL_DestroyWindow(imgui_debug_window);
    }
  }

  void ImGui_StartFrame()
  {
    if (!system::debug_mode)
    {
      return;
    }
    
    ImGuiIO& io = ImGui::GetIO();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();

    ImGui::NewFrame();
    ImGui::Begin("TermEngine - Debug Window", &utility::imgui_open, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);

    ImGui::SetWindowPos(ImVec2(0.0f, 0.0f));
    ImGui::SetWindowSize(imgui_window_size);

    ImGui::Text("FPS (Average): %.3f", system::GetAverageFPS());
    ImGui::Text("FPS (Target): %i", system::GetTargetFPS());
    ImGui::Text("Project: %s", system::scriptPath.c_str());

    ImGui::BeginTabBar("Lists");
  }

  void ImGui_EndFrame()
  {
    if (!system::debug_mode)
    {
      return;
    }
    
    ImGui::EndTabBar();
    ImGui::End();

    SDL_GL_MakeCurrent(imgui_debug_window, context);
    SDL_GL_SetSwapInterval(0);

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
    if (!system::debug_mode)
    {
      return;
    }

    if (events::KeyIsPressed("F3"))
    {
      if (!imgui_open)
      {
        SDL_ShowWindow(imgui_debug_window);
        imgui_open = true;
      }

      SDL_SetWindowSize(imgui_debug_window, (int)imgui_window_size.x, (int)imgui_window_size.y);
    }

    ImGuiIO& io = ImGui::GetIO();

    if (imgui_open && event.window.windowID == SDL_GetWindowID(imgui_debug_window))
    {
      ImGui_ImplSDL2_ProcessEvent(&event);

      if (event.type == SDL_WINDOWEVENT)
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

  void ImGui_UpdateInfo()
  {
    if (!system::debug_mode)
    {
      return;
    }

    if (ImGui::BeginTabItem("Game Windows"))
    {
      for (usertypes::GameWindowPtr& game_window : usertypes::game_window_list)
      {
        game_window->UpdateDebugInfo();
      }

      ImGui::EndTabItem();
    }

    if (ImGui::BeginTabItem("Game Scenes"))
    {
      for (auto& [ _, game_scene ] : usertypes::game_scene_list)
      {
        game_scene->UpdateDebugInfo();
      }

      ImGui::EndTabItem();
    }

    if (ImGui::BeginTabItem("Event Listeners"))
    {
      for (usertypes::EventListenerPtr& listener : usertypes::listener_list)
      {
        listener->UpdateDebugInfo();
      }

      ImGui::EndTabItem();
    }

    if (ImGui::BeginTabItem("Game Objects"))
    {
      ImGui::Text("Next ID: %lu", usertypes::BaseObject::GetNextId());

      for (usertypes::ObjectPtr& object : usertypes::object_list)
      {
          object->UpdateDebugInfo();
      }

      ImGui::EndTabItem();
    }

    if (ImGui::BeginTabItem("Resources"))
    {
      for (auto& [ _, resource ] : usertypes::resource_list)
      {
        resource->UpdateDebugInfo();
      }

      ImGui::EndTabItem();
    }
  }

  bool IsImguiWindow(uint32_t window_id)
  {
    return SDL_GetWindowID(imgui_debug_window) == window_id;
  }
}
