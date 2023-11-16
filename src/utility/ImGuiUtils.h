/// @author James Holtom

#ifndef IMGUI_UTILS_H
#define IMGUI_UTILS_H

#include <SDL2/SDL.h>
#include <imgui.h>
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_opengl3.h"
#include "misc/freetype/imgui_freetype.h"

namespace term_engine::utility {
  /// @brief The GLSL shader version that ImGui will use.
  constexpr char IMGUI_GL_VERSION[] = "#version 130";
  /// @brief The initial size of the debug window.
  constexpr ImVec2 DEBUG_WINDOW_SIZE = ImVec2(640, 480);

  /// @brief Is the debug window open?
  inline bool imgui_open = false;
  /// @brief The size of the debug/testing window, in pixels (px).
  inline ImVec2 imgui_window_size = DEBUG_WINDOW_SIZE;
  /// @brief The SDL handle to the debug/testing window.
  inline SDL_Window* imgui_debug_window = nullptr;

  /**
   * @brief Prepares ImGui for use.
   * 
   * @returns If ImGui is initialised.
   */
  bool InitImGui();

  /// @brief Destroys the debug window and shuts down ImGui.
  void CleanUpImGui();

  /// @brief Starts a new frame for drawing to the debug window.
  void ImGui_StartFrame();

  /// @brief Finished the frame and draws the debug window.
  void ImGui_EndFrame();

  /**
   * @brief Checks for events related to the use of the debug window.
   * 
   * @param[in] event The event to check.
   */
  void ImGui_DoEvents(const SDL_Event& event);

  /// @brief Updates the information on the debug window.
  void ImGui_UpdateInfo();

  /**
   * @brief Checks if the window with the given ID is being used for the debug window.
   * 
   * @param[in] window_id The window ID to check.
   * @returns If the window is being used for debugging.
   */
  bool IsImguiWindow(uint32_t window_id);
}

#endif // ! IMGUI_UTILS_H