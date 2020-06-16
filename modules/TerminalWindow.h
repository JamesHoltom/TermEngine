/*
 * Copyright 2020
 * James Holtom
 * MIT License
 */

#ifndef MOD_TERMINAL_WINDOW_H
#define MOD_TERMINAL_WINDOW_H

#include <functional>
#include <memory>
#include <vector>

#include "../types.h"
#include "../includes.h"
#include "../text/Glyph.h"
#include "../sdl.h"
#include "./CharacterCache.h"

namespace term_engine::modules {


  class TerminalWindow {
  public:
    TerminalWindow(CharacterCache* char_cache);
    ~TerminalWindow();

    int Update(const uint64_t& elapsedTime);
    int Resize();
    int Render(SDL_Window* window, SDL_Renderer* renderer);

    void ClearGlyphs();

    int SetGlyph(const int& x_cell, const int& y_cell, const Uint16& character, const SDL_Color& foreground, const SDL_Color& background);
    int SetGlyph(const int& x_cell, const int& y_cell, const Glyph& glyph);
    int SetGlyphs(std::function<int(std::vector<Glyph>&)> func);
    int FillGlyphs(std::function<Glyph()> generator);

    utilities::IntVector2D GetGridSize() const;

    utilities::IntVector2D GetGlyphSpacing() const;
    void SetGlyphSpacing(const int& x_spacing, const int& y_spacing);
    void SetGlyphSpacing(const int& spacing);

    utilities::IntVector2D GetGlyphPadding() const;
    void SetGlyphPadding(const int& x_padding, const int& y_padding);
    void SetGlyphPadding(const int& padding);

    utilities::IntVector2D GetWindowPosition() const;
    void SetWindowPosition(const int& x_pos, const int& y_pos);

    utilities::IntVector2D GetWindowSize() const;
    void SetWindowSize(const int& width, const int& height);
    void SetWindowSizeByCount();

    utilities::IntVector2D GetWindowCount() const;
    void SetWindowCount(const int& x_count, const int& y_count);

    utilities::IntVector2D GlobalToLocal(const int& global_x, const int& global_y);
    utilities::IntVector2D LocalToGlobal(const int& grid_x, const int& grid_y);

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
    std::vector<Glyph> glyphs_;
    SDL_Surface* glyph_surf_;
    CharacterCache* character_cache_;
  };
}

#endif // !MOD_TERMINAL_WINDOW_H
