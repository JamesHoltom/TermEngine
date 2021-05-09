/// @author James Holtom

#ifndef CL_ARGUMENTS_H
#define CL_ARGUMENTS_H

#include <filesystem>

namespace term_engine::system {
  /*
   * The command-line arguments are laid out as follows:
   * 
   * TermEngine.exe --project <script_path> --fullscreen
   */

  /// The path to the script to execute from the command line.
  extern std::filesystem::path script_path;

  /// Should the program start running in fullscreen?
  extern bool is_fullscreen;

  void GetCLArguments(int& argc, char**& argv);
}

#endif // ! CL_ARGUMENTS_H
