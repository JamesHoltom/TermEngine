/*
 * Copyright 2020
 * James Holtom
 * MIT License
 */

#ifndef FT_UTILS_H
#define FT_UTILS_H

#include <string>

#include <ft2build.h>
#include FT_FREETYPE_H
#include <spdlog/spdlog.h>

namespace FT {
  FT_Error Log(FT_Error result);
}

#endif // !FT_UTILS_H

