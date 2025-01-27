/// @author James Holtom

#ifndef LOG_FUNCTIONS_H
#define LOG_FUNCTIONS_H

#include <format>
#include <fstream>
#include <iostream>
#include <string>
#include <SDL2/SDL.h>

namespace term_engine::utility {
  /// @brief The default location to store log files at.
  constexpr char DEFAULT_LOG_LOCATION[] = "logs/term_engine.log";

  inline std::ofstream log_file;

  /// @brief Initialises the logger instance with file and console logging.
  void InitLogger();

  void ShutdownLogger();

  std::string GetFullFmt(const std::string& fmt, const std::string& type);

  template<typename... Args>
  void LogDebug(const std::string& fmt, Args&&... args)
  {
    std::string full_fmt = GetFullFmt(fmt, "DEBUG");

    std::string msg = std::vformat(full_fmt, std::make_format_args(args...));

    std::cout << msg;

    log_file << msg;
  }

  template<typename... Args>
  void LogInfo(const std::string& fmt, Args&&... args)
  {
    std::string full_fmt = GetFullFmt(fmt, "INFO");

    std::string msg = std::vformat(full_fmt, std::make_format_args(args...));

    std::cout << msg;

    log_file << msg;
  }

  template<typename... Args>
  void LogWarn(const std::string& fmt, Args&&... args)
  {
    std::string full_fmt = GetFullFmt(fmt, "WARNING");

    std::string msg = std::vformat(full_fmt, std::make_format_args(args...));

    std::cout << msg;

    log_file << msg;
  }

  template<typename... Args>
  void LogError(const std::string& fmt, Args&&... args)
  {
    std::string full_fmt = GetFullFmt(fmt, "ERROR");

    std::string msg = std::vformat(full_fmt, std::make_format_args(args...));

    std::cout << msg;

    log_file << msg;
  }

  /**
   * @brief Logs any keyboard-related events in an _SDL_Event_ object.
   * 
   * @param[in] event The _SDL_Event_ object to check for keyboard-related events.
   */
  void LogKeyboardEvents(const SDL_Event& event);

  /**
   * @brief Logs any text input-related events in an _SDL_Event_ object.
   * 
   * @param[in] event The _SDL_Event_ object to check for text input-related events.
   */
  void LogTextInputEvents(const SDL_Event& event);

  /**
   * @brief Logs any window-related events in an _SDL_Event_ object.
   * 
   * @param[in] event The _SDL_Event_ object to check for window-related events.
   */
  void LogWindowEvents(const SDL_Event& event);

  /// @brief Logs information about the currently bound VAO, such as what vertex attributes are bound, etc.
  void LogVAOData();

  /// @brief Logs information about the currently bound VBO.
  void LogVBOData();
}

#endif // ! LOG_FUNCTIONS_H
