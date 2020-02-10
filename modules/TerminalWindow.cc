#include <algorithm>

#include "TerminalWindow.h"

namespace term_engine::modules {
  TerminalWindow::TerminalWindow(CharacterCache* char_cache) :
    is_dirty_(false),
    needs_resizing_(false),
    glyph_x_count_(0),
    glyph_y_count_(0),
    glyph_x_spacing_(0),
    glyph_y_spacing_(0),
    glyph_x_padding_(0),
    glyph_y_padding_(0),
    bounds_({ 0, 0, 0, 0 }),
    character_cache_(char_cache),
    glyph_surf_(nullptr)
  {}
  TerminalWindow::~TerminalWindow() {
    character_cache_ = nullptr;

    SDL_FreeSurface(glyph_surf_);
    glyph_surf_ = nullptr;
  }

  int TerminalWindow::Update(const uint64_t& elapsed) {
    if (needs_resizing_) {
      Resize();
    }
    
    return 0;
  }
  
  int TerminalWindow::Resize() {
    if (needs_resizing_) {
      int newCount = glyph_x_count_ * glyph_y_count_;
      
      glyphs_.resize(newCount);
      std::fill(glyphs_.begin(), glyphs_.end(), utilities::nullGlyph);

      needs_resizing_ = false;
    }

    return 0;
  }

  int TerminalWindow::Render(SDL_Window* window, SDL_Renderer* renderer) {
    if (needs_resizing_) {
      return -1;
    }

    if (!is_dirty_) {
      return 0;
    }

    int x_pos = 0;
    int y_pos = 0;
    int count = 0;

    int glyph_width;
    int glyph_height;

    if (glyph_surf_ != nullptr) {
      SDL_FreeSurface(glyph_surf_);
    }

    character_cache_->GetGlyphDimensions(glyph_width, glyph_height);

    glyph_surf_ = SDL_CreateRGBSurfaceWithFormat(0, glyph_x_count_ * glyph_width, glyph_y_count_ * glyph_height, 32, SDL_PIXELFORMAT_RGBA32);

    if (glyph_surf_ == nullptr) {
      printf("Failed to create surface!");
      
      return -1;
    }

    std::for_each(glyphs_.begin(), glyphs_.end(), [this, &renderer, &x_pos, &y_pos, &count, &glyph_width, &glyph_height](utilities::Glyph it) {
      SDL_Rect fill_rect = { x_pos, y_pos, glyph_width + (glyph_x_padding_ * 2), glyph_height + (glyph_y_padding_ * 2) };
      SDL_FillRect(glyph_surf_, &fill_rect, SDL_MapRGBA(glyph_surf_->format, it.background.r, it.background.g, it.background.b, it.background.a));

      if (it.character != L'\u0000') {
        SDL_Rect dest_rect = { x_pos + glyph_x_padding_, y_pos + glyph_y_padding_, glyph_width, glyph_height };
        SDL_Surface* glyph = SDL_CreateRGBSurfaceWithFormat(0, glyph_width, glyph_height, 32, SDL_PIXELFORMAT_RGBA32);

        character_cache_->Render(glyph, glyph_x_padding_, glyph_y_padding_, it.character);
        if (SDL_SetSurfaceColorMod(glyph, it.foreground.r, it.foreground.g, it.foreground.b) == -1) {
          printf("Set FG failed for index %i!", count);
        }

        if (SDL_BlitSurface(glyph, nullptr, glyph_surf_, &dest_rect) == -1) {
          printf("Render failed for index %i!", count);
        }

        SDL_FreeSurface(glyph);
      }

      count++;

      if (count % glyph_x_count_ == 0 && count > 0) {
        x_pos = 0;
        y_pos += fill_rect.h + glyph_y_spacing_;
      }
      else {
        x_pos += fill_rect.w + glyph_x_spacing_;
      }
    });

    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, glyph_surf_);

    if (tex != nullptr) {
      SDL_RenderCopy(renderer, tex, nullptr, &bounds_);

      SDL_DestroyTexture(tex);
    } else {
      printf("Failed to create texture!");
      
      return -1;
    }

    is_dirty_ = false;

    return 0;
  }

  void TerminalWindow::ClearGlyphs() {
    glyphs_.clear();
  }

  int TerminalWindow::SetGlyph(const int& x_cell, const int& y_cell, const Uint32& character, const SDL_Color& foreground, const SDL_Color& background) {
    return SetGlyph(x_cell, y_cell, { character, foreground, background });
  }

  int TerminalWindow::SetGlyph(const int& x_cell, const int& y_cell, const utilities::Glyph& glyph) {
    int position = (y_cell * glyph_x_count_) + x_cell;
    
    if (position < 0 || position >= glyph_x_count_ * glyph_y_count_) {
      printf("Attempting to set an out-of-bounds glyph at index %i!", position);
      
      return -1;
    }

    glyphs_.at(position) = glyph;
    is_dirty_ = true;

    return 0;
  }

  int TerminalWindow::SetGlyphs(std::function<int(std::vector<utilities::Glyph>&)> func) {
    return func(glyphs_);
  }
  
  int TerminalWindow::FillGlyphs(std::function<utilities::Glyph()> generator) {
    std::generate(glyphs_.begin(), glyphs_.end(), generator);
    is_dirty_ = true;
    
    return 0;
  }

  void TerminalWindow::GetGlyphSpacing(int& x_spacing, int& y_spacing) const {
    x_spacing = glyph_x_spacing_;
    y_spacing = glyph_y_spacing_;
  }

  void TerminalWindow::SetGlyphSpacing(const int& x_spacing, const int& y_spacing) {
    glyph_x_spacing_ = x_spacing;
    glyph_y_spacing_ = y_spacing;

    is_dirty_ = true;
  }

  void TerminalWindow::SetGlyphSpacing(const int& spacing) {
    SetGlyphSpacing(spacing, spacing);
  }

  void TerminalWindow::GetGlyphPadding(int& x_padding, int& y_padding) const {
    x_padding = glyph_x_padding_;
    y_padding = glyph_y_padding_;
  }

  void TerminalWindow::SetGlyphPadding(const int& x_padding, const int& y_padding) {
    glyph_x_padding_ = x_padding;
    glyph_y_padding_ = y_padding;

    is_dirty_ = true;
  }

  void TerminalWindow::SetGlyphPadding(const int& padding) {
    SetGlyphSpacing(padding, padding);
  }

  void TerminalWindow::GetWindowPosition(int& x_pos, int& y_pos) const {
    x_pos = bounds_.x;
    y_pos = bounds_.y;
  }

  void TerminalWindow::SetWindowPosition(const int& x_pos, const int& y_pos) {
    bounds_.x = x_pos;
    bounds_.y = y_pos;
  }

  void TerminalWindow::GetWindowSize(int& width, int& height) const {
    width = bounds_.w;
    height = bounds_.h;
  }

  void TerminalWindow::SetWindowSize(const int& width, const int& height) {
    bounds_.w = width;
    bounds_.h = height;
  }

  void TerminalWindow::SetWindowSizeByCount() {
    int glyph_width;
    int glyph_height;
    character_cache_->GetGlyphDimensions(glyph_width, glyph_height);

    bounds_.w = (glyph_width + (glyph_x_padding_ * 2)) * glyph_x_count_ + (glyph_x_spacing_ * (glyph_x_count_ - 1));
    bounds_.h = (glyph_height + (glyph_y_padding_ * 2)) * glyph_y_count_ + (glyph_y_spacing_ * (glyph_y_count_ - 1));
  }

  void TerminalWindow::GetWindowCount(int& x_count, int& y_count) const {
    x_count = glyph_x_count_;
    y_count = glyph_y_count_;
  }

  void TerminalWindow::SetWindowCount(const int& x_count, const int& y_count) {
    glyph_x_count_ = x_count;
    glyph_y_count_ = y_count;

    needs_resizing_ = true;
  }
}
