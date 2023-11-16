/// @author James Holtom

#ifndef WINDOW_H
#define WINDOW_H

#include <forward_list>
#include <memory>
#include <string>
#include "GameWindow.h"
#include "../utility/GLUtils.h"
#include "../utility/SDLUtils.h"

namespace term_engine::usertypes {
  class Window;

  /// @brief The default position of the window.
  constexpr glm::ivec2 DEFAULT_WINDOW_POSITION(SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
  /// @brief The default size of the window.
  constexpr glm::ivec2 DEFAULT_WINDOW_SIZE(640, 512);
  /// @brief The default title for the window.
  constexpr char DEFAULT_WINDOW_TITLE[] = "TermEngine Game";
  /// @brief The default clear colour to use when refreshing the window.
  constexpr glm::vec4 DEFAULT_WINDOW_CLEAR_COLOUR(0.0f, 0.0f, 0.0f, 255.0f);

  /// @brief Unique pointer to a window.
  typedef std::unique_ptr<Window> WindowPtr;

  /// @brief The default window, which persists whenever the project is changed, to ensure an OpenGL context is always active.
  inline WindowPtr default_window = nullptr;

  /// @brief Used to handle a window on the screen.
  class Window {
  public:
    /**
     * @brief Constructs the window with the given parameters.
     * 
     * @param[in] game_window The GameWindow this window belongs to.
     * @param[in] position    The position of the window.
     * @param[in] size        The size of the window, in pixels (px).
     * @param[in] title       The title of the window.
     * @param[in] flags       The SDL_Window flags to set.
     */
    Window(GameWindow* game_window, const glm::ivec2& position, const glm::ivec2& size, const std::string& title, uint32_t flags);

    /// @brief Destroys the window.
    ~Window();

    /**
     * @brief Updates the window with events from SDL.
     * 
     * @param[in] event The SDL event to update the window with.
     */
    void DoEvents(const SDL_Event& event);

    /**
     * @brief Returns the window's ID.
     * 
     * @returns The window ID.
     */
    uint32_t GetId() const;

    /**
     * @brief Returns the SDL handle for the window.
     * 
     * @returns A raw pointer to the SDL window handle.
     */
    SDL_Window* GetWindow();

    /**
     * @brief Returns the position of the window.
     * 
     * @returns The position of the window.
     */
    glm::ivec2& GetPosition();

    /**
     * @brief Returns the size of the window.
     * 
     * @returns The size of the window, in pixels (px).
     */
    glm::ivec2& GetSize();

    /**
     * @brief Returns the clear colour of the window.
     * 
     * @returns The clear colour of the window.
     */
    glm::vec4& GetClearColour();

    /**
     * @brief Returns if wireframe rendering is enabled or not.
     * 
     * @returns If wireframe rendering is enabled.
     */
    bool IsWireframeEnabled() const;

    /**
     * @brief Returns the title of the window.
     * 
     * @returns The title of the window.
     */
    std::string GetTitle() const;

    /**
     * @brief Returns if the window is borderless.
     * 
     * @returns If the window is borderless.
     */
    bool IsBorderless() const;

    /**
     * @brief Returns if the window can be resized by the player.
     * 
     * @returns If the window is resizable.
     */
    bool IsResizable() const;

    /**
     * @brief Returns if the window is currently minimised.
     * 
     * @returns If the window is minimised.
     */
    bool IsMinimised() const;

    /**
     * @brief Returns if the window is currently maximised.
     * 
     * @returns If the window is maximised.
     */
    bool IsMaximised() const;

    /**
     * @brief Returns if the window has mouse focus.
     * 
     * @returns If the window has mouse focus.
     */
    bool IsInFocus() const;

    /**
     * @brief Returns if the window is grabbing the mouse, preventing it from moving outside the window.
     * 
     * @returns If the mouse has been grabbed.
     */
    bool IsMouseGrabbed() const;

    /**
     * @brief Repositions the window.
     * @note Calling this function also calls SetPosition().
     * 
     * @param[in] position The new position of the window.
     */
    void Reposition(const glm::ivec2& position);
    
    /// @brief Centers the window to the desktop.
    void CenterPosition();

    /**
     * @brief Resizes of the window.
     * @note Calling this function also calls SetSize().
     * 
     * @param[in] size The new size of the window.
     */
    void Resize(const glm::ivec2& size);
    
    /**
     * @brief Sets the position member of the window.
     * @details This function exists to update the position member, if the window moves due to an event. Call Reposition() to move the window.
     * 
     * @param[in] position The new position of the window.
     */
    void SetPosition(const glm::ivec2& position);

    /**
     * @brief Sets the size member of the window.
     * @details This function exists to update the size member, if the window changes size due to an event. Call Resize() to resize the window.
     * 
     * @param[in] size The new size of the window.
     */
    void SetSize(const glm::ivec2& size);

    /**
     * @brief Sets the clear colour of the window.
     * 
     * @param[in] colour The clear colour to use.
     */
    void SetClearColour(const glm::vec4& colour);

    /// @brief Disables wireframe rendering.
    void SetWireframe(bool flag);
    
    /**
     * @brief Sets the title of the window.
     * 
     * @param[in] title The title to set.
     */
    void SetTitle(const std::string& title) const;
    
    /**
     * @brief Sets if the window is borderless.
     * 
     * @param[in] flag The value to set.
     */
    void SetBorderless(bool flag) const;

    /**
     * @brief Sets if the window can be resized by the player.
     * 
     * @param[in] flag The value to set.
     */
    void SetResizable(bool flag) const;

    /// @brief Minimises the window.
    void Minimise() const;

    /// @brief Maximises the window.
    void Maximise() const;

    /// @brief Restores the window to its original size.
    void Restore() const;

    /// @brief Shows the window.
    void Show() const;

    /// @brief Hides the window.
    void Hide() const;
    
    /**
     * @brief Sets whether the window should grab the window or not.
     * 
     * @param[in] flag Whether the window should grab the mouse or not.
     */
    void SetMouseGrab(bool flag) const;
    
    /// @brief Sets the window as currently active.
    void Use() const;

    /// @brief Clears the window, ready for the next frame.
    void Clear() const;

    /// @brief Flips the window buffers, rendering the current frame's buffer to the screen.
    void Refresh() const;

    /**
     * @brief Sets the game window using this window.
     * @note This is solely used to assign the default window, whenever a project is loaded.
     * 
     * @param[in] ptr Raw pointer to the game window to set.
     */
    void SetGameWindow(GameWindow* ptr);

    /// @brief Updates the debugging information for this window.
    void UpdateDebugInfo() const;

    /**
     * @brief Returns if vsync is enabled.
     * @details Vsync fixes the framerate to the monitor's refresh rate.
     * 
     * @returns If vsync is enabled or not.
     */
    static bool IsVsyncEnabled();

    /**
     * @brief Enables/disables vsync.
     * @details If set to `-1`, this function will try to enable adaptive vsync. If this isn't supported, it will fall back to regular vsync.
     * 
     * @param[in] flag Whether to enable or disable vsync.
     */
    static void SetVsync(int flag);

  private:
    /// @brief The SDL handle to the window.
    SDL_Window* window_;
    /// @brief Raw pointer to the game window being rendered.
    GameWindow* game_window_;
    /// @brief Has the window been fully initialised?
    bool is_init_;
    /// @brief The position of the window, in pixels (px).
    glm::ivec2 position_;
    /// @brief The size of the window, in pixels (px).
    glm::ivec2 size_;
    /// @brief The colour to use for setting when clearing the window after every frame.
    glm::vec4 clear_colour_;
    /// @brief Whether to use wireframe rendering or not.
    uint32_t render_mode_;
    /// @brief Whether vsync is enabled/disabled.
    static int vsync_flag_;
  };

  /**
   * @brief Sets up the default window and the OpenGL context.
   * 
   * @returns If the default window and context were successfully set up.
   */
  bool InitDefaultWindow();

  /// @brief Destroys the default window and OpenGL context after use.
  void CleanUpDefaultWindow();
}

#endif // ! WINDOW_H
