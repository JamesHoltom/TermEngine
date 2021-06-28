#include <iostream>
#include <vector>
#include <spdlog/sinks/basic_file_sink.h>

#include "Logger.h"

namespace term_engine::logging {
  LoggerPtr logger;

  void InitLogger() {
    try {
      std::vector<spdlog::sink_ptr> sinks;

      auto console_sink = std::make_shared<spdlog::sinks::wincolor_stdout_sink_mt>();
      auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/term_engine.log", true);

      console_sink->set_level(spdlog::level::info);
      file_sink->set_level(spdlog::level::debug);

      sinks.push_back(console_sink);
      sinks.push_back(file_sink);

      logger = std::make_shared<spdlog::logger>("termengine", begin(sinks), end(sinks));
      logger->set_level(spdlog::level::debug);
    }
    catch (const spdlog::spdlog_ex& ex) {
      std::cout << "Log init failed: " << ex.what() << std::endl;
    }
  }
}