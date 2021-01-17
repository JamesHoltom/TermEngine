/// @author James Holtom

#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <filesystem>
#include <string>
#include <vector>

namespace term_engine::file {
  typedef std::vector<std::string> FileList;

  std::string ReadFromFile(const std::string& file_path);

  FileList GetFileList(const std::filesystem::path& directory);
  FileList GetFolderList(const std::filesystem::path& directory);

  std::filesystem::path GetRelative(const std::string& path);
}

#endif
