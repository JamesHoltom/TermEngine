/// @author James Holtom

#ifndef WINDOW_H
#define WINDOW_H

#include <string>
#include "../utility/GLUtils.h"
#include "../utility/SDLUtils.h"

namespace term_engine::rendering {
  /// @brief The default position of the window.
  constexpr glm::ivec2 DEFAULT_POSITION(SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
  /// @brief The default size of the window.
  constexpr glm::ivec2 DEFAULT_SIZE(640, 512);
  /// @brief The default title for the window.
  constexpr char DEFAULT_TITLE[] = "TermEngine Game";
  /// @brief The default clear colour to use when refreshing the window.
  constexpr glm::vec4 DEFAULT_WINDOW_CLEAR_COLOUR(0.0f, 0.0f, 0.0f, 255.0f);

  /// @brief Defines what can happen when the user closes the window.
  enum CloseLogic { HIDE = 0, CLOSE = 1, QUIT = 2 };

  /// @brief Used to handle a window on the screen.
  class GameWindow {
  public:
    /// @brief Constructs the window with the default parameters.
    GameWindow();

    /**
     * @brief Constructs the window with the given parameters.
     * 
     * @param[in] position  The position of the window.
     * @param[in] size      The size of the window, in pixels (px).
     * @param[in] title     The title of the window.
     */
    GameWindow(const glm::ivec2& position, const glm::ivec2& size, const std::string& title, uint32_t flags);

    /// @brief Destroys the window.
    ~GameWindow();

    /**
     * @brief Returns the window's ID.
     * 
     * @returns The window ID.
     */
    uint32_t GetId() const;

    /**
     * @brief Returns the position of the window.
     * 
     * @returns The position of the window.
     */
    glm::ivec2 GetPosition() const;

    /**
     * @brief Returns the size of the window.
     * 
     * @returns The size of the window, in pixels (px).
     */
    glm::ivec2 GetSize() const;

    /**
     * @brief Returns the title of the window.
     * 
     * @returns The title of the window.
     */
    std::string GetTitle() const;

    /**
     * @brief Returns the clear colour of the window.
     * 
     * @returns The clear colour of the window.
     */
    glm::vec4 GetClearColour() const;

    /**
     * @brief Returns whether the window will close, quit or hide upon user input.
     * 
     * @returns A value defining the window behaviour.
     */
    CloseLogic GetCloseBehaviour() const;

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
     * @brief Returns if the window is grabbing the mouse, preventing it from moving outside the window.
     * 
     * @returns If the mouse has been grabbed.
     */
    bool IsMouseGrabbed() const;

    /**
     * @brief Returns if wireframe rendering is enabled or not.
     * 
     * @returns If wireframe rendering is enabled.
     */
    bool IsWireframeEnabled() const;

    /**
     * @brief Set the position of the window.
     * 
     * @param[in] position The new position of the window.
     */
    void SetPosition(const glm::ivec2& position) const;
    
    /// @brief Centers the window to the desktop.
    void CenterPosition() const;
    
    /**
     * @brief Sets the size of the window.
     * 
     * @param[in] size The new size of the window.
     */
    void SetSize(const glm::ivec2& size) const;
    
    /**
     * @brief Sets the title of the window.
     * 
     * @param[in] title The title to set.
     */
    void SetTitle(const std::string& title) const;
    
    /**
     * @brief Sets the clear colour of the window.
     * 
     * @param[in] colour The clear colour to use.
     */
    void SetClearColour(const glm::vec4& colour);

    /**
     * @brief Sets if the window will close, quit or hide upon user input.
     * 
     * @param[in] behaviour A value defining the window behaviour.
     */
    void SetCloseBehaviour(CloseLogic behaviour);

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
    
    /// @brief Disables wireframe rendering.
    void SetWireframe(bool flag);

    /// @brief Sets the window as currently active.
    void Use() const;

    /// @brief Clears the window, ready for the next frame.
    void Clear() const;

    /// @brief Flips the window buffers, rendering the current frame's buffer to the screen.
    void Refresh() const;
    
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

    /// @brief Removes the OpenGL context for the window.
    static void CleanUpContext();

  private:
    /// @brief The SDL handle to the window.
    SDL_Window* window_;
    /// @brief Has the window been fully initialised?
    bool is_init_;
    /// @brief The colour to use for setting when clearing the window after every frame.
    glm::vec4 clear_colour_;
    /// @brief Whether to use wireframe rendering or not.
    uint32_t render_mode_;
    /// @brief The type of window behaviour when the user closes the window.
    CloseLogic close_logic_;
    /// @brief Flag used to indicate if an OpenGL context has been created. This is used to initialise GLEW when one is available.
    static bool is_context_created_;
    /// @brief The OpenGL context to bind windows to.
    static SDL_GLContext context_;
    /// @brief Whether Vsync is enabled/disabled.
    static int vsync_flag_;
  };
}

#endif // ! WINDOW_H
