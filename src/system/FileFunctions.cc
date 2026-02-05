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
#include "../scripting/ScriptingInterface.h"
#include "../utility/LogUtils.h"

namespace term_engine::system {
  std::filesystem::path SearchForProjectPath(const std::filesystem::path& filename)
  {
    if (std::filesystem::exists(filename))
    {
      return filename;
    }
    else if (filename.empty())
    {
      return "";
    }

#if defined(__linux__)
    const std::filesystem::path thisPath = std::filesystem::canonical("/proc/self/exe").parent_path();
#elif defined(_WIN32) || defined (_WIN64)
    const std::filesystem::path thisPath = GetModuleFileNameA(NULL);
#endif
    const std::filesystem::path rootPath = std::filesystem::current_path();
    const std::filesystem::path locations[] = {
      thisPath,
      thisPath / "projects",
      thisPath / "lua",
      rootPath,
      rootPath / "projects",
      rootPath / "lua"
    };

    for (const std::filesystem::path& location : locations)
    {
      const std::filesystem::path fullPath = location / filename;

      utility::LogDebug("Testing project path at {}.", fullPath.string());

      if (std::filesystem::exists(fullPath / "main.lua"))
      {
        utility::LogDebug("Found project path at {}.", fullPath.string());

        return fullPath;
      }
    }

    utility::LogWarn("Could not find project directory {}!", filename.string());

    return "";
  }

  std::filesystem::path SearchForResourcePath(const std::string& filename)
  {
    if (std::filesystem::exists(filename))
    {
      return filename;
    }
    else if (filename.empty())
    {
      return "";
    }

#if defined(__linux__)
    const std::filesystem::path thisPath = std::filesystem::canonical("/proc/self/exe").parent_path();
#elif defined(_WIN32) || defined (_WIN64)
    const std::filesystem::path thisPath = GetModuleFileNameA(NULL);
#endif
    const std::filesystem::path locations[] = {
#if defined(__linux__)
      "/usr/share/fonts",
      "/usr/local/share/fonts",
#elif defined(_WIN32) || defined (_WIN64)
      "C:/Windows/Fonts",
#elif defined(__APPLE__) && defined(__MACH__)
      "/System/Library/Fonts"
#endif
      thisPath,
      thisPath / "resources",
      scripting::project_path,
      scripting::project_path / "resources"
    };

    for (const std::filesystem::path& location : locations)
    {
      const std::filesystem::path fullPath = location / filename;

      utility::LogDebug("Testing resource path at {}.", fullPath.string());

      if (std::filesystem::exists(fullPath))
      {
        utility::LogDebug("Found resource path at {}.", fullPath.string());

        return fullPath;
      }
    }

    utility::LogWarn("Could not find resource \"{}\"!", filename);

    return "";
  }

  std::string ReadFile(const std::string& filename)
  {
    std::filesystem::path filepath = SearchForResourcePath(filename);

    if (filepath.empty())
    {
      utility::LogWarn("Could not find file to read data from!");
      
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
      utility::LogError("Failed to read from file {}.", filepath.string());

      return "";
    }
  }

  void WriteFile(const std::string& filename, const std::string& data, bool append)
  {
    if (filename.empty())
    {
      utility::LogWarn("No file has been selected to write/append data to!");
      
      return;
    }

    std::filesystem::path filepath = scripting::project_path / filename;
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
      utility::LogError("Failed to write to file {}.", filepath.string());
    }
  }

  bool FileExists(const std::string& filename)
  {
    return SearchForResourcePath(filename) != "";
  }

  FileList GetFileList(const std::string& directory)
  {
    FileList file_list;
    std::filesystem::path dirpath = scripting::project_path / directory;

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
    std::filesystem::path dirpath = scripting::project_path / directory;

    for (std::filesystem::directory_entry file : std::filesystem::directory_iterator(dirpath)) {
      if (file.is_directory()) {
        folder_list.push_back(std::string(file.path().stem().string() + "/"));
      }
    }

    return folder_list;
  }
}
