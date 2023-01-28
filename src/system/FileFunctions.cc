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
#include "../utility/SpdlogUtils.h"

namespace term_engine::system {
  std::filesystem::path project_path;

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

      utility::logger->debug("Testing location {}...", fullPath);

      if (std::filesystem::exists(fullPath))
      {
        utility::logger->debug("Found project path at {}.", fullPath);

        return fullPath;
      }
    }

    utility::logger->warn("Could not find project directory {}!", filename);

    return "";
  }

  std::filesystem::path SearchForResourcePath(const std::string& filename)
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
      rootPath / "resources",
      // TODO: Make the project path available here. Perhaps finish project-related code and make it available there?
      project_path,
      project_path / "resources"
    };

    for (const std::filesystem::path& location : locations)
    {
      const std::filesystem::path fullPath = location / filename;

      utility::logger->debug("Testing resource path at {}.", fullPath);

      if (std::filesystem::exists(fullPath))
      {
        utility::logger->debug("Found resource path at {}.", fullPath);

        return fullPath;
      }
    }

    utility::logger->warn("Could not find resource \"{}\"!", filename);

    return "";
  }

  std::string ReadFile(const std::string& filename)
  {
    std::filesystem::path filepath = SearchForResourcePath(filename);

    if (filepath == "")
    {
      utility::logger->warn("Could not find file to read data from!");
      
      return "";
    }

    std::ifstream file_stream;
    std::stringstream output_stream;

    file_stream.open(filepath);

    if (file_stream.is_open())
    {
      output_stream << file_stream.rdbuf();
      file_stream.close();

      return output_stream.str();
    }
    else
    {
      utility::logger->error("Failed to read from file {}.", filepath);

      return "";
    }
  }

  void WriteFile(const std::string& filename, const std::string& data, const bool& append)
  {
    if (filename == "")
    {
      utility::logger->warn("No file has been selected to write/append data to!");
      
      return;
    }

    std::filesystem::path filepath = project_path / filename;
    std::ofstream file_stream;
    std::ios_base::openmode mode = append ? std::ios::app : std::ios::trunc;

    file_stream.open(filepath, mode);

    if (file_stream.is_open())
    {
      file_stream << data;
      file_stream.close();
    }
    else
    {
      utility::logger->error("Failed to write to file {}.", filepath);
    }
  }

  bool FileExists(const std::string& filename)
  {
    return SearchForResourcePath(filename) != "";
  }

  FileList GetFileList(const std::string& directory)
  {
    FileList file_list;
    std::filesystem::path dirpath = project_path / directory;

    for (std::filesystem::directory_entry file : std::filesystem::directory_iterator(dirpath)) {
      if (!file.is_directory()) {
        file_list.push_back(file.path().stem().string());
      }
    }

    return file_list;
  }

  FileList GetFolderList(const std::string& directory)
  {
    FileList folder_list;
    std::filesystem::path dirpath = project_path / directory;

    for (std::filesystem::directory_entry file : std::filesystem::directory_iterator(dirpath)) {
      if (file.is_directory()) {
        folder_list.push_back(std::string(file.path().stem().string() + "/"));
      }
    }

    return folder_list;
  }
}
