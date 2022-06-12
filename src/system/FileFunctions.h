/// @author James Holtom

#ifndef FILE_FUNCTIONS_H
#define FILE_FUNCTIONS_H

#include <filesystem>
#include <string>
#include <vector>

namespace term_engine::system {
  /// Used to store a list of file/folder locations.
  typedef std::vector<std::string> FileList;

  /// Reads the contents of a file.
  /**
   * @param[in] directory The file to read.
   * @returns The contents of the file.
   */
  std::string ReadFromFile(const std::filesystem::path& directory);

  /// Retrieves a list of files within the given directory.
  /**
   * @param[in] directory The directory to search for files within.
   * @returns The list of file names found in the directory.
   */
  FileList GetFileList(const std::filesystem::path& directory);

  /// Retrieves a list of folders within the given directory.
  /**
   * @param[in] directory The directory to search for folders within.
   * @returns The list of folder names found in the directory.
   */
  FileList GetFolderList(const std::filesystem::path& directory);

  /// Gets the directory that the TermEngine executable is located in.
  /**
   * @returns The root directory.
   */
  std::filesystem::path GetRootPath();

  /// Finds the "main.lua" file at the given project directory in the system's default location, or in the projects folder.
  /**
   * If the file isn't found anywhere, a blank string is returned.
   * @param[in] filename The path of the project file.
   * @returns The absolute path to the project file, or a blank string if not found.
   */
  std::filesystem::path SearchForProjectPath(const std::filesystem::path& directory);

  /// Finds the given font file in the system's default location, or in the resources folder.
  /**
   * If the file isn't found anywhere, a blank string is returned.
   * @param[in] filename The path of the font file.
   * @returns The absolute path to the font file, or a blank string if not found.
   */
  std::filesystem::path SearchForFontPath(const std::string& filename);

  /// Finds the given background file in the system's default location, or in the resources folder.
  /**
   * If the file isn't found anywhere, a blank string is returned.
   * @param[in] filename The path of the background file.
   * @returns The absolute path to the background file, or a blank string if not found.
   */
  std::filesystem::path SearchForBackgroundPath(const std::string& filename);
}

namespace File {
  /// Converts a directory to its absolute equivalent, based on the program's current path.
  /**
   * For example, if the program is located at "C:\TermEngine", the path "./projects/example" will convert to "C:\TermEngine\projects\example".
   * 
   * @param[in] directory The directory to convert.
   * @returns The converted directory.
   */
  std::filesystem::path Absolute(const std::filesystem::path& directory);

  /// Converts a directory to its relative equivalent, based on the program's current path.
  /**
   * For example, if the program is located at "C:\TermEngine", the path "C:\TermEngine\projects\example" will convert to "./projects/example".
   *
   * @param[in] directory The directory to convert.
   * @returns The converted directory.
   */
  std::filesystem::path Relative(const std::filesystem::path& directory);

  /// Checks if the given file directory exists.
  /**
   * @param[in] directory The file directory to check.
   * @returns If the file directory exists or not.
   */
  bool Exists(const std::filesystem::path& directory);
}

#endif // ! FILE_FUNCTIONS_H
