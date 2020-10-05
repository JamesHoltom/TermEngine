/*
 * Copyright 2020
 * James Holtom
 * MIT License
 */

#ifndef SPDLOG_UTILS_H
#define SPDLOG_UTILS_H

#ifdef TERM_DEBUG
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#else
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_WARN
#endif

#include <spdlog/spdlog.h>

namespace spdlog {
  void InitSpdLog();
}

#endif // ! SPDLOG_UTILS_H
