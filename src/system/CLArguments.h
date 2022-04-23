/// @author James Holtom

#ifndef CL_ARGUMENTS_H
#define CL_ARGUMENTS_H

#include <filesystem>

namespace term_engine::system {
  /**
   * The command-line arguments are laid out as follows:
   * 
   * TermEngine.exe --project [scriptPath] --fullscreen
   */

  /// The path to the script to execute from the command line.
  extern std::filesystem::path scriptPath;
  /// Should the program start running in fullscreen?
  extern bool isFullscreen;

  /// Gets the command-line arguments passed to the program.
  /**
   * @param[in] argc The number of arguments passed.
   * @param[in] argv The text content of the arguments.
   */
  void GetCLArguments(int& argc, char**& argv);
}

#endif // ! CL_ARGUMENTS_H
