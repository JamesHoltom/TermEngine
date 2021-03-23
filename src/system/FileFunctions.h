/// @author James Holtom

#ifndef FILE_FUNCTIONS_H
#define FILE_FUNCTIONS_H

#include <filesystem>
#include <string>
#include <vector>

namespace term_engine::system {
  typedef std::vector<std::string> FileList;

  bool IsValidPath(const std::filesystem::path& file_path);

  std::string ReadFromFile(const std::string& file_path);

  FileList GetFileList(const std::filesystem::path& directory);
  FileList GetFolderList(const std::filesystem::path& directory);

  std::filesystem::path GetRelative(const std::string& path);
}

#endif // ! FILE_FUNCTIONS_H
