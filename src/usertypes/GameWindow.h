/// @author James Holtom

#ifndef GAME_WINDOW_H
#define GAME_WINDOW_H

#include <forward_list>
#include <memory>
#include "Background.h"
#include "Flaggable.h"
#include "GameScene.h"
#include "Window.h"
#include "resources/Font.h"
#include "resources/ShaderProgram.h"
#include "../rendering/Buffer.h"
#include "../utility/SDLUtils.h"

namespace term_engine::usertypes {
  class GameWindow;
  class GameScene;
  class Window;

  /// @brief Smart pointer to a game window.
  typedef std::shared_ptr<GameWindow> GameWindowPtr;
  /// @brief Used to store a list of game windows.
  typedef std::forward_list<GameWindowPtr> GameWindowList;

  /// @brief Defines what can happen when the user closes the window.
  enum CloseLogic { HIDE = 0, CLOSE = 1, QUIT = 2 };

  /// @brief A list of smart pointers to the game windows.
  inline GameWindowList game_window_list;
  
  /// @brief Manages a single window, that handles what font/game scene/shaders/background are drawn to it.
  class GameWindow : public Flaggable {
  private:
    /// @brief Raw pointer to the game scene being drawn.
    GameScene* game_scene_;
    /// @brief Raw pointer to the window.
    Window* window_;
    /// @brief Flag to determine if this window is the default one, and should persist when switching projects.
    bool is_default_window_;
    /// @brief The background texture rendered behind the game scene.
    Background background_;
    /// @brief The font used to render characters to the game scene.
    Font* font_;
    /// @brief The shader program used to render backgrounds to the game scene.
    ShaderProgram* background_shader_program_;
    /// @brief The shader program used to render characters to the game scene.
    ShaderProgram* text_shader_program_;
    /// @brief Buffer to store the background vertices.
    rendering::Buffer background_buffer_;
    /// @brief Buffer to store the character vertices.
    rendering::Buffer text_buffer_;
    /// @brief The font size to render characters at, in pixels (px).
    uint32_t font_size_;
    /// @brief Flag to check if the window is closing.
    bool is_closing_;
    /// @brief The type of window behaviour when the user closes the window.
    CloseLogic close_logic_;
    /// @brief Flag to indicate if the user wants to quit the program.
    static bool quit_flag;

  public:
    /**
     * @brief Constructs the game window.
     * 
     * @param[in] is_default Is this the default window, and this window should persist after changing projects?
     */
    GameWindow(bool is_default);

    /// @brief Destroys the game window.
    ~GameWindow();

    /**
     * @brief Returns the game scene being rendered to the window.
     * 
     * @return A raw pointer to the game scene.
     */
    GameScene* GetGameScene();

    /**
     * @brief Returns the window where the game scene is rendered to.
     * 
     * @return A raw pointer to the window object.
     */
    Window* GetWindow();

    /**
     * @brief Returns the background rendered behind the game scene.
     * 
     * @returns A raw pointer to the background object.
     */
    Background* GetBackground();

    /**
     * @brief Returns the font used by the game scene.
     * 
     * @returns A raw pointer to the font.
     */
    Font* GetFont();

    /**
     * @brief Returns the font size this window renders characters at.
     * 
     * @returns The font size.
     */
    uint32_t GetFontSize() const;

    /**
     * @brief Returns the shader program used to render backgrounds to the window.
     * 
     * @returns A raw pointer to the shader program.
     */
    ShaderProgram* GetBackgroundShader();

    /**
     * @brief Returns the shader program used to render characters to the window.
     * 
     * @returns A raw pointer to the shader program.
     */
    ShaderProgram* GetTextShader();

    /**
     * @brief Returns whether the window will close, quit or hide upon user input.
     * 
     * @returns A value defining the window behaviour.
     */
    CloseLogic GetCloseBehaviour() const;

    /**
     * @brief Sets the game scene that is drawn to this window.
     * 
     * @param[in] game_scene The game scene to set, or it's string index.
     */
    void SetGameScene(GameSceneVariant game_scene);

    /**
     * @brief Sets the font used by the window.
     * 
     * @param[in] font The font resource, or it's string index.
     */
    void SetFont(FontVariant font);

    /**
     * @brief Sets the font size this window renders characters at.
     * 
     * @param[in] font_size The new font size.
     */
    void SetFontSize(uint32_t font_size);

    /**
     * @brief Sets the shader used to render backgrounds to the window.
     * 
     * @param[in] shader The shader program, or it's string index.
     */
    void SetBackgroundShader(ShaderProgramVariant shader);

    /**
     * @brief Sets the shader program used to render characters to the window.
     * 
     * @param[in] shader The shader program, or it's string index.
     */
    void SetTextShader(ShaderProgramVariant shader);

    /**
     * @brief Sets if the window will close, quit or hide upon user input.
     * 
     * @param[in] behaviour A value defining the window behaviour.
     */
    void SetCloseBehaviour(CloseLogic behaviour);

    /// @brief Unloads and re-loads the current game scene, if set.
    void ReloadGameScene();

    /**
     * @brief Updates the game scene (if set) and draws the character/background data to the window.
     * 
     * @param[in] timestep The time since the last frame was run.
     */
    void Update(float timestep);

    /**
     * @brief Returns if the window is closing.
     * 
     * @returns Whether the window is closing.
     */
    bool IsClosing() const;

    /// @brief Marks the window to be closed.
    void Close();

    /// @brief Resizes the window to fit the game scene's character map.
    void ResizeToFitCharacterMap();

    /// @brief Resizes the game scene's character map to fit the window.
    void ResizeToFitWindow();

    /**
     * @brief Sets the projection matrix for the window, according to the given size.
     * 
     * @param[in] window_size The size of the window, in pixels (px).
     */
    void SetProjection(const glm::ivec2& window_size);

    /// @brief Updates the debugging information for this game window.
    void UpdateDebugInfo();

    /**
     * @brief Returns if the user has quit the program.
     * 
     * @returns If the program is quitting.
     */
    static bool IsQuitting();

    /// @brief Informs the program that the user is quitting.
    static void Quit();
  };

  /**
   * @brief Adds a game window to the list.
   * 
   * @returns A raw pointer to the window.
   */
  GameWindow* AddGameWindow();

  /**
   * @brief Adds the default game window to the list.
   * @note The default window persists whenever the project is changed, to ensure an OpenGL context is always active.
   * 
   * @returns A raw pointer to the default window.
   */
  GameWindow* AddDefaultGameWindow();

  /**
   * @brief Returns the game window with the given window ID.
   * 
   * @param[in] window_id The ID to the window.
   * @returns A raw pointer to the window, or a null pointer if not found.
   */
  GameWindow* GetGameWindowByWindowId(uint32_t window_id);

  /**
   * @brief Updates the game window with events from SDL.
   * 
   * @param[in] event The SDL event to update the window with.
   */
  void DoGameWindowEvents(const SDL_Event& event);

  /**
   * @brief Updates each game window.
   * 
   * @param[in] timestep The time since the last frame was run.
   */
  void UpdateGameWindows(float timestep);

  /// @brief Clears all game windows that are flagged for removal from the list.
  void ClearFlaggedGameWindows();

  /// @brief Clears all game windows from the list.
  void ClearAllGameWindows();
}

#endif // ! GAME_WINDOW_H
