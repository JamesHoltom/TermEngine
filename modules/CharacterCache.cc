#include <algorithm>
#include <numeric>
#include <stdio.h>

#include "CharacterCache.h"
#include "../utility/CharacterRange.h"

namespace term_engine::modules {
  CharacterCache::CharacterCache() :
    is_loaded_(false),
    font_name_(""),
    font_size_(0),
    glyph_width_(0),
    glyph_set_(nullptr)
  {}
  CharacterCache::CharacterCache(const std::string& font_name, const int& font_size) :
    is_loaded_(false),
    font_name_(font_name),
    font_size_(font_size),
    glyph_width_(0)
  {
    Load(font_name);
  }
  CharacterCache::~CharacterCache() {
    Delete();
  }

  inline bool CharacterCache::isLoaded() const {
    return is_loaded_;
  }

  int CharacterCache::Load(const std::string& font_name) {
    if (is_loaded_) {
      return -1;
    }

    font_name_ = font_name;

    TTF_Font* font = TTF_OpenFont(font_name_.c_str(), font_size_);

    if (TTF_FontFaceIsFixedWidth(font)) {
      printf_s("Font is monospaced!\n");
    }
    else {
      printf_s("Font is not monospaced.\n");
    }

    int glyph_x = 0;

    glyph_set_ = TTF_RenderUNICODE_Blended(font, utilities::Symbols.data(), { 255, 255, 255, 255 });

    std::for_each(utilities::Symbols.begin(), utilities::Symbols.end() - 1, [this, &font, &glyph_x](Uint16 it) {
      int offset;
      TTF_GlyphMetrics(font, it, nullptr, nullptr, nullptr, nullptr, &offset);

      glyph_map_[it] = { glyph_x, 0, offset, font_size_ };
      glyph_x += offset;

      if (offset > glyph_width_) {
        glyph_width_ = offset;
      }
    });

    printf_s("Glyph width is set to %i.\n", glyph_width_);

    TTF_CloseFont(font);
    font = nullptr;

    is_loaded_ = true;

    return 0;
  }

  int CharacterCache::Delete() {
    if (!is_loaded_) {
      return -1;
    }

    font_name_ = "";

    glyph_width_ = 0;

    SDL_FreeSurface(glyph_set_);
    glyph_set_ = nullptr;
    glyph_map_.clear();

    is_loaded_ = false;

    return 0;
  }

  int CharacterCache::Render(SDL_Surface* destination, const int& x, const int& y, const Uint16& character) {
    if (is_loaded_) {
      SDL_Rect src_rect = glyph_map_.at(character);
      SDL_Rect dest_rect = { x, y, 0, 0 };

      if (SDL_BlitSurface(glyph_set_, &src_rect, destination, &dest_rect) != 0) {
        printf_s("Failed to render! %s\n", SDL_GetError());

        return -1;
      }

      return 0;
    }
    else {
      return -1;
    }
  }

  int CharacterCache::GetGlyphDimensions(int& width, int& height) const {
    if (is_loaded_) {
      width = glyph_width_;
      height = font_size_;

      return 0;
    }
    else {
      return -1;
    }
  }
}
