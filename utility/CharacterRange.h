/*
 * Copyright 2020
 * James Holtom
 * MIT License
 */

#ifndef UTIL_CHARACTER_RANGE_H
#define UTIL_CHARACTER_RANGE_H

#include <array>

#include "SDLIncludes.h"

namespace term_engine::utilities {
  constexpr int LatinSetLength = 94;
  constexpr int StructureSymbolsSetLength = 256;
  constexpr int ArrowsSetLength = 112;
  constexpr int MiscSymbolsSetLength = 448;
  constexpr int MiscSymbolsExtendedSetLength = 256;

  void InitCharacterRanges();

  extern std::array<Uint16, LatinSetLength + StructureSymbolsSetLength + ArrowsSetLength + MiscSymbolsSetLength + MiscSymbolsExtendedSetLength + 1> Symbols;
}

#endif // ! UTIL_CHARACTER_RANGE_H
