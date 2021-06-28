/// @author James Holtom

#ifndef LOGGER_H
#define LOGGER_H

#include <memory>
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

namespace term_engine::logging {
  /// Smart pointer to share a single instance of a `Logger`.
  typedef std::shared_ptr<spdlog::logger> LoggerPtr;

  /// The instance of the logger.
  extern LoggerPtr logger;

  /// Initialises the logger instance with file and console logging.
  void InitLogger();
}

#endif // ! LOGGER_H
