#include <string>
#include <cxxopts.hpp>
#include "CLArguments.h"
#include "../logging/Logger.h"

namespace term_engine::system {
  std::filesystem::path scriptPath;

  void GetCLArguments(int& argc, char**& argv)
  {
    cxxopts::Options options("TermEngine", "Test.");
    options.add_options()
      ("project", "The project to execute.", cxxopts::value<std::string>()->default_value(""));
    options.parse_positional({ "project" });

    try
    {
      auto result = options.parse(argc, argv);

      scriptPath = std::filesystem::path(result["project"].as<std::string>());
    }
    catch (cxxopts::OptionParseException& ex)
    {
      logging::logger->error("Failed to parse command-line arguments. Error: {}", std::string(ex.what()));
    }
    catch (cxxopts::OptionSpecException& ex)
    {
      logging::logger->error("Failed to define command-line arguments. Error: {}", std::string(ex.what()));
    }
  }
}