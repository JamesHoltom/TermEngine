#include "FontAtlas.h"
#include "../logging/Logger.h"

namespace term_engine::fonts {
  FontAtlas::FontAtlas(const FT_Library& library, const std::string& font_path, const int& font_size):
    font_path_(font_path),
    font_size_(font_size),
    texture_id_(0),
    texture_position_(0),
    texture_max_layers_(0)
  {
    Init(library);
  }

  FontAtlas::FontAtlas(const FT_Library& library, const FontAtlasKey& font_key) :
    font_path_(font_key.first),
    font_size_(font_key.second),
    texture_id_(0),
    texture_position_(0),
    texture_max_layers_(0)
  {
    Init(library);
  }

  FontAtlas::~FontAtlas()
  {
    FT::Log(FT_Done_Face(font_face_));
  }

  std::string FontAtlas::GetFontPath() const
  {
    return font_path_;
  }

  int FontAtlas::GetFontSize() const
  {
    return font_size_;
  }

  GLint FontAtlas::GetCharacter(const FT_ULong& character)
  {
    auto found_char = font_atlas_.find(character);

    if (found_char == font_atlas_.end()) {
      return LoadChar(character);
    }
    else {
      return found_char->second;
    }
  }

  const GLuint& FontAtlas::GetTextureId() const
  {
    return texture_id_;
  }

  glm::vec2 FontAtlas::GetTextureSize() const
  {
    return texture_size_;
  }

  void FontAtlas::Use() const
  {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D_ARRAY, texture_id_);
  }

  void FontAtlas::Unuse() const
  {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
  }

  void FontAtlas::Init(const FT_Library& library)
  {
    /*
     * Depending on the version of OpenGL, there is a limit of how many layers can be added to an array texture.
     * In OpenGL 4.4 and below, this is 512. In 4.5 onwards, this has been raised to 2048.
     *
     * Source: https://www.khronos.org/opengl/wiki/Array_Texture#Limitations
     */
    glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &texture_max_layers_);

    FT::Log(FT_New_Face(library, font_path_.c_str(), 0, &font_face_));
    FT::Log(FT_Set_Pixel_Sizes(font_face_, font_size_, font_size_));

    /*
     * Using the bounding box, we can get the size of the texture, including ascender and descender.
     * The bounding box is in an unscaled, 1/64th of a pixel per unit format, and must be converted before use.
     */
    texture_size_ = glm::vec2((font_face_->bbox.xMax - font_face_->bbox.xMin) >> 6, (font_face_->bbox.yMax - font_face_->bbox.yMin) >> 6);

    glGenTextures(1, &texture_id_);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D_ARRAY, texture_id_);
    glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGB8, (GLsizei)texture_size_.x, (GLsizei)texture_size_.y, texture_max_layers_);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    logging::logger->debug("Texture #{} created with dimensions of {}, {} and {} layers.", texture_id_, texture_size_.x, texture_size_.y, texture_max_layers_);
    logging::logger->debug("OpenGL has a max limit of {} layers in a texture array.", texture_max_layers_);
  }

  GLint FontAtlas::LoadChar(const FT_ULong& character)
  {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D_ARRAY, texture_id_);

    if (FT::Log(FT_Load_Char(font_face_, character, FT_LOAD_RENDER)) == FT_Err_Ok) {
      // Glyph metrics are stored in an unscaled, 1/64th of a pixel per unit format, and must be converted before use.
      GLuint glyph_width = font_face_->glyph->bitmap.width;
      GLuint glyph_height = font_face_->glyph->bitmap.rows;
      GLuint glyph_left = ((GLsizei)texture_size_.x - glyph_width + (font_face_->glyph->metrics.horiBearingX >> 6)) / 2;
      GLuint glyph_top = (font_face_->ascender - font_face_->glyph->metrics.horiBearingY) >> 6;

      glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
      glPixelStorei(GL_UNPACK_ROW_LENGTH, glyph_width);
      glPixelStorei(GL_UNPACK_IMAGE_HEIGHT, glyph_height);

      glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, glyph_left, glyph_top, texture_position_, glyph_width, glyph_height, 1, GL_RED, GL_UNSIGNED_BYTE, font_face_->glyph->bitmap.buffer);
      glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

      auto new_glyph = font_atlas_.emplace(std::make_pair(character, texture_position_));

      logging::logger->debug("Created character, \'{}\' with dimensions {},{} at layer {} and added to cache.", (char)character, glyph_width, glyph_height, texture_position_);

      texture_position_++;

      return new_glyph.first->second;
    }
    else {
      logging::logger->error("Failed to load character \'{}\' from font \'{}\'.", character, font_path_);

      return -1;
    }
  }
}
