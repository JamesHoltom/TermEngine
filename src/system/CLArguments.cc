#include "CLArguments.h"

#include <cxxopts.hpp>

#include "../logging/Logger.h"

namespace term_engine::system {
  std::string script_path;
  bool is_fullscreen;

  void GetCLArguments(int& argc, char**& argv)
  {
    cxxopts::Options options("TermEngine", "Test.");
    options.add_options()
      ("script_path", "Path to the script to execute.", cxxopts::value<std::string>()->default_value(""))
      ("fullscreen", "Start in fullscreen?");

    try
    {
      auto result = options.parse(argc, argv);

      script_path = result["script_path"].as<std::string>();
      is_fullscreen = result["fullscreen"].as<bool>();
    }
    catch (cxxopts::OptionException ex)
    {
      logging::logger->error("Failed to retrieve command-line arguments. Error: ", ex.what());
    }
  }
}