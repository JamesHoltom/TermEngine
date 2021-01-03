/*
 * Copyright 2020
 * James Holtom
 * MIT License
 */

#ifndef LOGGER_H
#define LOGGER_H

#include <memory>
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

#include "../utility/LoggingUtils.h"

namespace term_engine::logging {
  typedef std::shared_ptr<spdlog::logger> LoggerPtr;

  void InitLogger();

  extern LoggerPtr logger;
}

#endif // ! LOGGER_H
