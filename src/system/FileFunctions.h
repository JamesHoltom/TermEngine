/// @author James Holtom

#ifndef FILE_FUNCTIONS_H
#define FILE_FUNCTIONS_H

#include <filesystem>
#include <string>
#include <vector>

namespace term_engine::system {
  /// Used to store a list of file/folder locations.
  typedef std::vector<std::string> FileList;

  /// Gets the directory that the TermEngine executable is located in.
  /**
   * @returns The root directory.
   */
  std::filesystem::path GetRootPath();

  /// Finds the "main.lua" file at the given project directory in the system's default location, or in the projects folder.
  /**
   * If the file isn't found anywhere, a blank string is returned.
   * @param[in] directory The directory to the project file.
   * @returns The absolute path to the project file, or a blank string if not found.
   */
  std::filesystem::path SearchForProjectPath(const std::filesystem::path& directory);

  /// Finds the given resource file (e.g. background, file, font, etc.) in the system's default location, or in the resources folder.
  /**
   * If the file isn't found anywhere, a blank string is returned.
   * @param[in] filename The path of the background file.
   * @returns The absolute path to the background file, or a blank string if not found.
   */
  std::filesystem::path SearchForResourcePath(const std::string& filename);

  std::string ReadFile(const std::string& filename);

  void WriteFile(const std::string& filename, const std::string& data, const bool& append);

  bool FileExists(const std::string& filename);

  /// Retrieves a list of files within the given directory.
  /**
   * @param[in] directory The directory to search for files within.
   * @returns The list of file names found in the directory.
   */
  FileList GetFileList(const std::string& directory);

  /// Retrieves a list of folders within the given directory.
  /**
   * @param[in] directory The directory to search for folders within.
   * @returns The list of folder names found in the directory.
   */
  FileList GetFolderList(const std::string& directory);
}

#endif // ! FILE_FUNCTIONS_H
