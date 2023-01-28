/// @author James Holtom

#ifndef FILE_FUNCTIONS_H
#define FILE_FUNCTIONS_H

#include <filesystem>
#include <string>
#include <vector>

namespace term_engine::system {
  /// @brief Used to store a list of file/folder locations.
  typedef std::vector<std::string> FileList;

  /// @brief Stores the filepath to the current project.
  extern std::filesystem::path project_path;

  /**
   * @brief Gets the directory that the TermEngine executable is located in.
   * 
   * @returns The root directory.
   */
  std::filesystem::path GetRootPath();

  /**
   * @brief Finds the "main.lua" file at the given project directory in the system's default location, or in the projects folder.
   * @details If the file isn't found anywhere, a blank string is returned.
   * 
   * @param[in] directory The directory to the project file.
   * @returns The absolute path to the project file, or a blank string if not found.
   */
  std::filesystem::path SearchForProjectPath(const std::filesystem::path& directory);

  /**
   * @brief Finds the given resource file (e.g. background, file, font, etc.) in the system's default location, or in the resources folder.
   * @details If the file isn't found anywhere, a blank string is returned.
   * 
   * @param[in] filename The path of the background file.
   * @returns The absolute path to the background file, or a blank string if not found.
   */
  std::filesystem::path SearchForResourcePath(const std::string& filename);

  /**
   * @brief Reads and returns the contents of a file.
   * @details If the file isn't found anywhere, a blank string is returned.
   * 
   * @param[in] filename The path to the file.
   * @returns The contents of the file, or an blank string if not found.
   */
  std::string ReadFile(const std::string& filename);

  /**
   * @brief Writes a string of data to a file.
   * @details This function only allows file writing to the project folder; writing to files outside of a project is not allowed.
   * 
   * @param[in] filename  The path to the file.
   * @param[in] data      The data to write.
   * @param[in] append    Should the data be appended to any existing contents in the file?
   */
  void WriteFile(const std::string& filename, const std::string& data, const bool& append);

  /**
   * @brief Returns if a file with the given path exists.
   * @details This function looks within the OS's font path, the program's root path and the project path.
   * 
   * @param[in] filename The path to the file.
   * @returns If the file exists.
   */
  bool FileExists(const std::string& filename);

  /**
   * @brief Retrieves a list of files within the given directory.
   * 
   * @param[in] directory The directory to search for files within.
   * @returns The list of file names found in the directory.
   */
  FileList GetFileList(const std::string& directory);

  /**
   * @brief Retrieves a list of folders within the given directory.
   * 
   * @param[in] directory The directory to search for folders within.
   * @returns The list of folder names found in the directory.
   */
  FileList GetFolderList(const std::string& directory);
}

#endif // ! FILE_FUNCTIONS_H
