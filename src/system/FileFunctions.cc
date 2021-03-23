#include <fstream>
#include <iostream>
#include <sstream>

#include "FileFunctions.h"
#include "../logging/Logger.h"

namespace term_engine::system {
  bool IsValidPath(const std::filesystem::path& file_path)
  {
    return std::filesystem::exists(file_path);
  }

  std::string ReadFromFile(const std::string& file_path)
  {
    std::ifstream file_stream;
    std::stringstream output_stream;

    file_stream.exceptions(std::ifstream::badbit | std::ifstream::failbit);

    try {
      file_stream.open(file_path);

      output_stream << file_stream.rdbuf();

      file_stream.close();

      return output_stream.str();
    }
    catch (std::ifstream::failure exception) {
      logging::logger->error("Failed to read from file \'{}\'.\nError: {}", file_path, exception.what());

      return "";
    }
  }

  FileList GetFileList(const std::filesystem::path& directory)
  {
    FileList file_list;

    for (std::filesystem::directory_entry file : std::filesystem::directory_iterator(directory)) {
      if (!file.is_directory()) {
        file_list.push_back(file.path().stem().string());
      }
    }

    return file_list;
  }

  FileList GetFolderList(const std::filesystem::path& directory)
  {
    FileList folder_list;

    for (std::filesystem::directory_entry file : std::filesystem::directory_iterator(directory)) {
      if (file.is_directory()) {
        folder_list.push_back(std::string(file.path().stem().string() + "/"));
      }
    }

    return folder_list;
  }

  std::filesystem::path GetRelative(const std::string& path)
  {
    std::error_code err;
    std::filesystem::path relative = std::filesystem::relative(std::filesystem::path(path), err);

    if (err) {
      logging::logger->error(err.message());
    }

    return relative;
  }
}
