#include <numeric>

#include "CharacterRange.h"

namespace term_engine::utilities {
  void InitCharacterRanges() {
    int offset = 0;

    std::iota(Symbols.begin(), Symbols.begin() + LatinSetLength, L'\u0021');
    offset += LatinSetLength;

    std::iota(Symbols.begin() + offset, Symbols.begin() + offset + StructureSymbolsSetLength, L'\u2500');
    offset += StructureSymbolsSetLength;

    std::iota(Symbols.begin() + offset, Symbols.begin() + offset + ArrowsSetLength, L'\u2190');
    offset += ArrowsSetLength;

    std::iota(Symbols.begin() + offset, Symbols.begin() + offset + MiscSymbolsSetLength, L'\u2600');
    offset += MiscSymbolsSetLength;

    std::iota(Symbols.begin() + offset, Symbols.begin() + offset + MiscSymbolsExtendedSetLength, L'\u2B00');
    offset += MiscSymbolsExtendedSetLength;

    Symbols.at(offset) = L'\u0000';
  }

  std::array<Uint16, LatinSetLength + StructureSymbolsSetLength + ArrowsSetLength + MiscSymbolsSetLength + MiscSymbolsExtendedSetLength + 1> Symbols;
}