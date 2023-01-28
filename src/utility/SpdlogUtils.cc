#include <iostream>
#include <vector>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include "SpdlogUtils.h"
#include "../system/FileFunctions.h"

namespace term_engine::utility {
  LoggerPtr logger;

  void InitLogger()
  {
    try
    {
      std::vector<spdlog::sink_ptr> sinks;

      auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
      auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(system::GetRootPath() / std::string(DEFAULT_LOG_LOCATION), true);

      console_sink->set_level(spdlog::level::info);
      file_sink->set_level(spdlog::level::debug);

      sinks.push_back(console_sink);
      sinks.push_back(file_sink);

      logger = std::make_shared<spdlog::logger>("termengine", begin(sinks), end(sinks));
      logger->set_level(spdlog::level::debug);
    }
    catch (const spdlog::spdlog_ex& ex)
    {
      std::cout << "Log init failed: " << ex.what() << std::endl;
    }
  }
}
