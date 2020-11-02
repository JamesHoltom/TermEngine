#include "spdlogUtils.h"

namespace spdlog {
  void InitSpdLog() {
#ifdef TERM_DEBUG
    spdlog::set_level(spdlog::level::trace);
#else
    spdlog::set_level(spdlog::level::info);
#endif
  }
}