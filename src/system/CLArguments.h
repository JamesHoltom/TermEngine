/// @author James Holtom

#ifndef CL_ARGUMENTS_H
#define CL_ARGUMENTS_H

#include <filesystem>

namespace term_engine::system {
  /**
   * @details The command-line arguments are laid out as follows:
   * 
   * TermEngine.exe [scriptPath]
   */

  /// @brief The path to the script to execute from the command line.
  inline std::filesystem::path scriptPath;

  /**
   * @brief Gets the command-line arguments passed to the program.
   * 
   * @param[in] argc The number of arguments passed.
   * @param[in] argv The text content of the arguments.
   */
  void GetCLArguments(int& argc, char**& argv);
}

#endif // ! CL_ARGUMENTS_H
