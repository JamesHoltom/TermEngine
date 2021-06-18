#include "FontAtlas.h"
#include "../data/Uniform.h"
#include "../logging/Logger.h"

namespace term_engine::fonts {
  int Init()
  {
    /*
     * Depending on the version of OpenGL, there is a limit of how many layers can be added to an array texture.
     * In OpenGL 4.4 and below, this is 512. In 4.5 onwards, this has been raised to 2048.
     *
     * Source: https://www.khronos.org/opengl/wiki/Array_Texture#Limitations
     */
    glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &texture_max_layers);

    logging::logger->debug("OpenGL has a max limit of {} layers in a texture array.", texture_max_layers);

    return SetFont(DEFAULT_FONT, DEFAULT_FONT_SIZE);
  }

  void CleanUp()
  {
    glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
    glDeleteTextures(1, &texture_id);
    FT::Log(FT_Done_Face(font_face));
  }

  GLint GetCharacter(const FT_ULong& character)
  {
    auto found_char = font_atlas.find(character);

    if (found_char == font_atlas.end()) {
      return LoadChar(character);
    }
    else {
      return found_char->second;
    }
  }

  GLint LoadChar(const FT_ULong& character)
  {
    if (FT::Log(FT_Load_Char(font_face, character, FT_LOAD_RENDER)) == FT_Err_Ok) {
      // Glyph metrics are stored in an unscaled, 1/64th of a pixel per unit format, and must be converted before use.
      GLuint glyph_width = font_face->glyph->bitmap.width;
      GLuint glyph_height = font_face->glyph->bitmap.rows;
      GLuint glyph_left = ((GLsizei)texture_size.x - glyph_width + (font_face->glyph->metrics.horiBearingX >> 6)) / 2;
      GLuint glyph_top = (font_face->ascender - font_face->glyph->metrics.horiBearingY) >> 6;

      glBindTexture(GL_TEXTURE_2D_ARRAY, texture_id);
      glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
      glPixelStorei(GL_UNPACK_ROW_LENGTH, glyph_width);
      glPixelStorei(GL_UNPACK_IMAGE_HEIGHT, glyph_height);

      glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, glyph_left, glyph_top, glyph_count, glyph_width, glyph_height, 1, GL_RED, GL_UNSIGNED_BYTE, font_face->glyph->bitmap.buffer);
      glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

      auto new_glyph = font_atlas.emplace(std::make_pair(character, glyph_count));

      logging::logger->debug("Created character, \'{}\' with dimensions {},{} at layer {} and added to cache.", (char)character, glyph_width, glyph_height, glyph_count);

      glyph_count++;

      return new_glyph.first->second;
    }
    else {
      logging::logger->error("Failed to load character \'{}\' from font \'{}\'.", character, font_path);

      return -1;
    }
  }

  int SetFont(const std::string& filename, const FT_UInt& size)
  {
    if (filename != font_path) {
      if (font_path != "") {
        if (FT::Log(FT_Done_Face(font_face)) != FT_Err_Ok) {
          logging::logger->error("Failed to remove font face.");

          return 1;
        }

        glDeleteTextures(1, &texture_id);
      }

      if (FT::Log(FT_New_Face(FT::font_library, filename.c_str(), 0, &font_face)) != FT_Err_Ok) {
        logging::logger->error("Failed to create font face.");

        return 1;
      }

      glGenTextures(1, &texture_id);
    }

    if (filename != font_path && size != font_size) {
      if (FT::Log(FT_Set_Pixel_Sizes(font_face, size, size)) != FT_Err_Ok) {
        logging::logger->error("Failed to set font size.");

        return 1;
      }
    }

    /*
     * Using the bounding box, we can get the size of the texture, including ascender and descender.
     * The bounding box is in an unscaled, 1/64th of a pixel per unit format, and must be converted before use.
     */
    texture_size = glm::vec2((font_face->bbox.xMax - font_face->bbox.xMin) >> 6, (font_face->bbox.yMax - font_face->bbox.yMin) >> 6);

    glBindTexture(GL_TEXTURE_2D_ARRAY, texture_id);
    glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGB8, (GLsizei)texture_size.x, (GLsizei)texture_size.y, texture_max_layers);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

    data::SetFontSize(texture_size);

    font_path = filename;
    font_size = size;

    logging::logger->debug("Texture #{} created with dimensions of {}, {} and {} layers.", texture_id, texture_size.x, texture_size.y, texture_max_layers);

    return 0;
  }

  void Use()
  {
    glBindTexture(GL_TEXTURE_2D_ARRAY, texture_id);
  }

  void Unuse()
  {
    glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
  }

  std::string font_path;
  FT_UInt font_size;
  FT_Face font_face;
  GlyphList font_atlas;
  GLuint texture_id;
  GLuint glyph_count;
  glm::vec2 texture_size;
  GLint texture_max_layers;
}
