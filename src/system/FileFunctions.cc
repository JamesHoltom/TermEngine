#include <fstream>
#include <iostream>
#include <sstream>

#ifdef linux
#include <unistd.h>
#include <linux/limits.h>
#elif defined(_WIN32) || defined (WIN32)
#include <windows.h>
#endif

#include "FileFunctions.h"
#include "CLArguments.h"
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

  std::filesystem::path GetRootPath()
  {
#ifdef linux
    char szPath[PATH_MAX];
    ssize_t count = readlink( "/proc/self/exe", szPath, PATH_MAX );

    if( count < 0 || count >= PATH_MAX )
    {
        return "";
    }
    szPath[count] = '\0';
#elif defined(_WIN32) || defined (WIN32)
    wchar_t szPath[MAX_PATH];
    GetModuleFileNameW( NULL, szPath, MAX_PATH );
#endif

    return std::filesystem::path{ szPath }.parent_path() / "";
  }

  std::filesystem::path SearchForProjectPath(const std::filesystem::path& filename)
  {
    if (std::filesystem::exists(filename))
    {
      return filename;
    }

    const std::filesystem::path rootPath = GetRootPath();
    const std::filesystem::path locations[] = {
      rootPath,
      rootPath / "projects",
      rootPath / "lua"
    };

    for (const std::filesystem::path& location : locations)
    {
      const std::filesystem::path fullPath = location / filename;

      if (std::filesystem::exists(fullPath))
      {
        return fullPath;
      }
    }

    logging::logger->warn("Could not find project directory {}!", filename);

    return "";
  }

  std::filesystem::path SearchForFontPath(const std::string& filename)
  {
    const std::filesystem::path rootPath = GetRootPath();
    const std::filesystem::path locations[] = {
#ifdef linux
      "/usr/share/fonts/",
      "/usr/local/share/fonts/",
#elif defined(_WIN32) || defined (WIN32)
      "C:/Windows/Fonts/",
#endif
      rootPath,
      scriptPath
    };

    for (const std::filesystem::path& location : locations)
    {
      const std::filesystem::path fullPath = location / filename;

      logging::logger->info("Testing location {}...", fullPath);

      if (std::filesystem::exists(fullPath))
      {
        return fullPath;
      }
    }

    logging::logger->warn("Could not find font \"{}\"!", filename);

    return "";
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
