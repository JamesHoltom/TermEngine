#include <string>
#include <cxxopts.hpp>
#include "CLArguments.h"
#include "../utility/LogUtils.h"

namespace term_engine::system {
  void GetCLArguments(int& argc, char**& argv)
  {
    cxxopts::Options options("TermEngine", "Game engine that focuses on creating text-based games.");
    options.add_options()
      ("project", "The project to execute.", cxxopts::value<std::string>()->default_value(""))
      ("debug", "Enable debugging options?", cxxopts::value<bool>());
    options.parse_positional({ "project" });

    try
    {
      auto result = options.parse(argc, argv);

      scriptPath = std::filesystem::path(result["project"].as<std::string>());
      debug_mode = result["debug"].as<bool>();
    }
    catch (cxxopts::exceptions::parsing& ex)
    {
      utility::LogError("Failed to parse command-line arguments. Error: {}", ex.what());
    }
    catch (cxxopts::exceptions::specification& ex)
    {
      utility::LogError("Failed to define command-line arguments. Error: {}", ex.what());
    }
  }
}