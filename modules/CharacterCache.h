/*
 * Copyright 2020
 * James Holtom
 * MIT License
 */

#ifndef MOD_CHARACTER_CACHE_H
#define MOD_CHARACTER_CACHE_H

#include <memory>
#include <string>
#include <unordered_map>

#include "../types.h"
#include "../includes.h"
#include "../text/Glyph.h"
#include "../sdl.h"

namespace term_engine::modules {
  class CharacterCache {
  public:
    CharacterCache();
    CharacterCache(const std::string& font_name, const int& font_size);
    CharacterCache(const CharacterCache&) = delete;
    ~CharacterCache();

    CharacterCache& operator=(const CharacterCache&) = delete;

    inline bool isLoaded() const;
    int Load(const std::string& font_name);
    int Delete();

    int Render(SDL_Surface* destination, const int& x, const int& y, const Uint16& character);

    utilities::IntVector2D GetGlyphDimensions() const;

  private:
    bool is_loaded_;
    std::string font_name_;
    int font_size_;
    int glyph_width_;
    SDL_Surface* glyph_set_;
    std::unordered_map<Uint16, SDL_Rect> glyph_map_;
  };
}

#endif // !MOD_CHARACTER_CACHE_H
