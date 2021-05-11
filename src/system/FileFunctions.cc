#include <fstream>
#include <iostream>
#include <sstream>

#include "FileFunctions.h"
#include "../logging/Logger.h"

namespace term_engine::system {
  std::string ReadFromFile(const std::filesystem::path& directory)
  {
    std::ifstream file_stream;
    std::stringstream output_stream;

    file_stream.exceptions(std::ifstream::badbit | std::ifstream::failbit);

    try {
      file_stream.open(directory);

      output_stream << file_stream.rdbuf();

      file_stream.close();

      return output_stream.str();
    }
    catch (std::ifstream::failure exception) {
      logging::logger->error("Failed to read from file \'{}\'.\nError: {}", directory, exception.what());

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
}

namespace File {
  std::filesystem::path Absolute(const std::filesystem::path& directory)
  {
    std::error_code err;
    std::filesystem::path absolute = std::filesystem::absolute(directory, err);

    if (err) {
      term_engine::logging::logger->error(err.message());
    }

    return absolute;
  }

  std::filesystem::path Relative(const std::filesystem::path& directory)
  {
    std::error_code err;
    std::filesystem::path relative = std::filesystem::relative(directory, err);

    if (err) {
      term_engine::logging::logger->error(err.message());
    }

    return relative;
  }

  bool Exists(const std::filesystem::path& directory)
  {
    std::error_code err;
    bool result = std::filesystem::exists(directory, err);

    if (err) {
      term_engine::logging::logger->error(err.message());
    }

    return result;
  }
}
