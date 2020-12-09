#include "FontAtlas.h"
#include "../logging/Logger.h"
#include "../utility/FTUtils.h"
#include "../utility/GLUtils.h"

namespace term_engine::fonts {
  FontAtlas::FontAtlas(const FT_Library& library, const std::string& font_path, const int& font_size):
    font_path_(font_path),
    font_size_(font_size),
    texture_id_(0),
    texture_offset_(0),
    texture_max_layers_(0)
  {
    GLint max_layers;
    glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &max_layers);

    if (MAX_GLYPH_LAYERS > max_layers) {
      logging::logger->warn("Maximum glyph layers of {} exceeds OpenGL\'s limit of {}.", MAX_GLYPH_LAYERS, texture_max_layers_);

      texture_max_layers_ = max_layers;
    }
    else {
      texture_max_layers_ = MAX_GLYPH_LAYERS;
    }

    FT::Log(FT_New_Face(library, font_path_.c_str(), 0, &font_face_));
    FT::Log(FT_Set_Pixel_Sizes(font_face_, font_size_, font_size_));

    texture_size_ = glm::uvec2((font_face_->bbox.xMax - font_face_->bbox.xMin) >> 6, (font_face_->bbox.yMax - font_face_->bbox.yMin) >> 6);

    glGenTextures(1, &texture_id_);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D_ARRAY, texture_id_);
    glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGB8, texture_size_.x, texture_size_.y, texture_max_layers_);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    logging::logger->debug("Texture #{} created with dimensions of {}, {} and {} layers.", texture_id_, texture_size_.x, texture_size_.y, texture_max_layers_);
    logging::logger->debug("OpenGL has a max limit of {} layers in a texture array.", max_layers);
  }

  FontAtlas::~FontAtlas() {
    FT::Log(FT_Done_Face(font_face_));
  }

  std::string FontAtlas::GetFontPath() const {
    return font_path_;
  }

  int FontAtlas::GetFontSize() const {
    return font_size_;
  }

  GLint FontAtlas::GetCharacter(const FT_ULong& character) {
    auto found_char = font_atlas_.find(character);

    if (found_char == font_atlas_.end()) {
      return LoadChar(character);
    }
    else {
      return found_char->second;
    }
  }

  const GLuint& FontAtlas::GetTextureId() const {
    return texture_id_;
  }

  glm::uvec2 FontAtlas::GetTextureSize() const {
    return texture_size_;
  }

  void FontAtlas::Use() const {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D_ARRAY, texture_id_);
  }

  void FontAtlas::Unuse() const {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
  }

  GLint FontAtlas::LoadChar(const FT_ULong& character) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D_ARRAY, texture_id_);

    if (FT::Log(FT_Load_Char(font_face_, character, FT_LOAD_RENDER)) == FT_Err_Ok) {
      GLuint glyph_width = font_face_->glyph->bitmap.width;
      GLuint glyph_height = font_face_->glyph->bitmap.rows;
      GLuint glyph_left = (texture_size_.x - glyph_width + (font_face_->glyph->metrics.horiBearingX >> 6)) / 2;
      GLuint glyph_top = (font_face_->ascender - font_face_->glyph->metrics.horiBearingY) >> 6;

      glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
      glPixelStorei(GL_UNPACK_ROW_LENGTH, glyph_width);
      glPixelStorei(GL_UNPACK_IMAGE_HEIGHT, glyph_height);

      glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, glyph_left, glyph_top, texture_offset_, glyph_width, glyph_height, 1, GL_RED, GL_UNSIGNED_BYTE, font_face_->glyph->bitmap.buffer);
      glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

      auto new_glyph = font_atlas_.emplace(std::make_pair(character, texture_offset_));

      logging::logger->debug("Created character, \'{}\' with dimensions {},{} at layer {} and added to cache.", (char)character, glyph_width, glyph_height, texture_offset_);

      texture_offset_++;

      return new_glyph.first->second;
    }
    else {
      logging::logger->error("Failed to load character \'{}\' from font \'{}\'.", character, font_path_);

      return -1;
    }
  }
}
