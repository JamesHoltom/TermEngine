/*
 * Copyright 2020
 * James Holtom
 * MIT License
 */

#ifndef FONT_ATLAS_H
#define FONT_ATLAS_H

#include <memory>
#include <string>
#include <unordered_map>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <gl/glew.h>
#include <glm/glm.hpp>

namespace term_engine::fonts {
  typedef std::unordered_map<FT_UInt32, GLuint> GlyphAtlas;

  constexpr int MAX_GLYPH_LAYERS = 512;

  class FontAtlas {
  public:
    FontAtlas(const FT_Library& library, const std::string& font_path, const int& glyph_size);
    ~FontAtlas();

    std::string GetFontPath() const;
    int GetFontSize() const;

    GLint GetCharacter(const FT_ULong &character);
    const GLuint& GetTextureId() const;
    glm::uvec2 GetTextureSize() const;

    void Use() const;
    void Unuse() const;

  private:
    std::string font_path_;
    int font_size_;

    FT_Face font_face_;
    GlyphAtlas font_atlas_;

    GLuint texture_id_;
    GLuint texture_offset_;
    glm::uvec2 texture_size_;
    GLint texture_max_layers_;

    GLint LoadChar(const FT_ULong& character);
  };

  typedef std::shared_ptr<FontAtlas> FontAtlasPtr;
  typedef std::unordered_map<std::string, FontAtlasPtr> FontAtlasList;
  typedef std::pair<std::string, FontAtlasPtr> FontAtlasIter;
}

#endif // ! FONT_ATLAS_H
