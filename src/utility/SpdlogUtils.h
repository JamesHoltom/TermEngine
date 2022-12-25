/// @author James Holtom

#ifndef LOGGER_H
#define LOGGER_H

#include <memory>
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

namespace logging {
  /// @brief The default location to store log files at.
  constexpr char DEFAULT_LOG_LOCATION[] = "logs/term_engine.log";

  /// @brief Smart pointer to share a single instance of a `Logger`.
  typedef std::shared_ptr<spdlog::logger> LoggerPtr;

  /// @brief The instance of the logger.
  extern LoggerPtr logger;

  /// @brief Initialises the logger instance with file and console logging.
  void InitLogger();
}

#endif // ! LOGGER_H
