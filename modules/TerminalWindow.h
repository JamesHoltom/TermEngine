/*
 * Copyright 2020
 * James Holtom
 * MIT License
 */

#ifndef MOD_TERMINAL_WINDOW_H
#define MOD_TERMINAL_WINDOW_H

#include <memory>
#include <vector>
#include <SDL.h>

#include "../utility/Glyph.h"
#include "../utility/SDLUtilities.h"
#include "./CharacterCache.h"

namespace term_engine::modules {
  class TerminalWindow {
  public:
    TerminalWindow(CharacterCache* char_cache);
    ~TerminalWindow();

    int Update(const uint64_t& elapsedTime);
    int Render(SDL_Window* window, SDL_Renderer* renderer);

    void ClearGlyphs();

    int SetGlyph(const int& x_cell, const int& y_cell, const Uint32& character, const SDL_Color& foreground, const SDL_Color& background);
    int SetGlyph(const int& x_cell, const int& y_cell, const utilities::Glyph& glyph);

    void GetGlyphSpacing(int& x_spacing, int& y_spacing) const;
    void SetGlyphSpacing(const int& x_spacing, const int& y_spacing);
    void SetGlyphSpacing(const int& spacing);

    void GetGlyphPadding(int& x_padding, int& y_padding) const;
    void SetGlyphPadding(const int& x_padding, const int& y_padding);
    void SetGlyphPadding(const int& padding);

    void GetWindowPosition(int& x_pos, int& y_pos) const;
    void SetWindowPosition(const int& x_pos, const int& y_pos);

    void GetWindowSize(int& width, int& height) const;
    void SetWindowSize(const int& width, const int& height);
    void SetWindowSizeByCount();

    void GetWindowCount(int& x_count, int& y_count) const;
    void SetWindowCount(const int& x_count, const int& y_count);

  private:
    bool is_dirty_;
    bool needs_resizing_;

    int glyph_x_count_;
    int glyph_y_count_;

    int glyph_x_spacing_;
    int glyph_y_spacing_;

    int glyph_x_padding_;
    int glyph_y_padding_;

    SDL_Rect bounds_;
    std::vector<utilities::Glyph> glyphs_;
    SDL_Surface* glyph_surf_;
    CharacterCache* character_cache_;
  };
}

#endif // !MOD_TERMINAL_WINDOW_H
