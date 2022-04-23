#include <array>
#include <filesystem>

#include "FontAtlas.h"
#include "../data/Uniform.h"
#include "../logging/Logger.h"
#include "../system/FileFunctions.h"

namespace term_engine::fonts {
  std::string font_path;
  FT_UInt font_size;
  FT_Face font_face;
  GlyphList font_atlas;
  GLuint texture_id;
  GLuint glyph_count;
  glm::vec2 texture_size;
  GLint texture_max_layers;

  bool Init()
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
      return _LoadChar(character);
    }
    else {
      return found_char->second;
    }
  }

  GLint _LoadChar(const FT_ULong& character)
  {
    if (_CreateCharTexture(character, glyph_count)) {
      auto new_glyph = font_atlas.emplace(std::make_pair(character, glyph_count));

      glyph_count++;

      return new_glyph.first->second;
    }
    else {
      return -1;
    }
  }

  bool _CreateCharTexture(const FT_ULong& character, const GLint texture_layer)
  {
    FT_BBox bbox;

    if (character == 0)
    {
      return true;
    }

    // TODO: Fix this calculating the wrong values.
    if (FT::Log(FT_Load_Char(font_face, character, FT_LOAD_RENDER)) == FT_Err_Ok)
    {
      if (FT::Log(FT_Set_Pixel_Sizes(font_face, texture_size.x, texture_size.y)) != FT_Err_Ok) {
        logging::logger->error("Failed to set font size.");

        return 1;
      }

      // Glyph metrics are stored in an unscaled, 1/64th of a pixel per unit format, and must be converted before use.
      GLuint glyph_width = font_face->glyph->bitmap.width;
      GLuint glyph_height = font_face->glyph->bitmap.rows;
      GLuint glyph_left = ((GLsizei)texture_size.x - glyph_width + (font_face->glyph->metrics.horiBearingX >> 6)) / 2;
      GLuint glyph_top = ((font_face->size->metrics.ascender - font_face->glyph->metrics.horiBearingY) >> 6);

      glBindTexture(GL_TEXTURE_2D_ARRAY, texture_id);
      glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
      glPixelStorei(GL_UNPACK_ROW_LENGTH, glyph_width);
      glPixelStorei(GL_UNPACK_IMAGE_HEIGHT, glyph_height);

      glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, glyph_left, glyph_top, texture_layer, glyph_width, glyph_height, 1, GL_RED, GL_UNSIGNED_BYTE, font_face->glyph->bitmap.buffer);
      glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

      logging::logger->debug("Created character, \'{}\' for font \'{}\' with dimensions {},{} at pos {},{} at layer {} and added to cache.", (char)character, font_path, glyph_width, glyph_height, glyph_left, glyph_top, texture_layer);

      return true;
    }
    else
    {
      logging::logger->error("Failed to load character \'{}\' from font \'{}\'.", character, font_path);

      return false;
    }
  }

  bool SetFont(const std::string& filename, const FT_UInt& size)
  {
    const std::filesystem::path fullFontPath = system::SearchForFontPath(filename);

    if (!std::filesystem::exists(fullFontPath)) {
      logging::logger->warn("Attempting to set font to one that doesn't exist: {}", fullFontPath);

      return false;
    }

    if (fullFontPath != font_path) {
      if (font_path != "") {
        if (FT::Log(FT_Done_Face(font_face)) != FT_Err_Ok) {
          logging::logger->error("Failed to remove font face.");

          return false;
        }
      }

      if (FT::Log(FT_New_Face(FT::font_library, fullFontPath.c_str(), 0, &font_face)) != FT_Err_Ok) {
        logging::logger->error("Failed to create font face.");

        return false;
      }
    }

    if (FT::Log(FT_Set_Pixel_Sizes(font_face, 0, size)) != FT_Err_Ok) {
      logging::logger->error("Failed to set font size.");

      return false;
    }

    glDeleteTextures(1, &texture_id);
    glGenTextures(1, &texture_id);

    font_path = fullFontPath;
    font_size = size;

    /*
     * Using the bounding box, we can get the size of the texture, including ascender and descender.
     * The bounding box is in an unscaled, 1/64th of a pixel per unit format, and must be converted before use.
     */
    //texture_size = glm::vec2(((font_face->bbox.xMax - font_face->bbox.xMin) >> 6) + 1, ((font_face->bbox.yMax - font_face->bbox.yMin) >> 6) + 1);
    texture_size = glm::vec2(size);

    glBindTexture(GL_TEXTURE_2D_ARRAY, texture_id);
    glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGB8, (GLsizei)texture_size.x, (GLsizei)texture_size.y, texture_max_layers);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

    for (auto& glyph : font_atlas) {
      _CreateCharTexture(glyph.first, glyph.second);
    }

    data::SetFontSize(texture_size);

    logging::logger->debug("Texture #{} created with dimensions of {}, {} and {} layers.", texture_id, texture_size.x, texture_size.y, texture_max_layers);

    return true;
  }

  std::string GetFontPath()
  {
    return font_path;
  }

  std::string GetDefaultFontPath()
  {
    return std::string(DEFAULT_FONT);
  }

  int GetFontSize()
  {
    return font_size;
  }

  int GetDefaultFontSize()
  {
    return DEFAULT_FONT_SIZE;
  }

  glm::vec2 GetTextureSize()
  {
    return texture_size;
  }
}
