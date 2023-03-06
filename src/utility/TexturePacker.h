/// @author James Holtom

#ifndef GLYPH_PACKING_UTILS_H
#define GLYPH_PACKING_UTILS_H

#include <memory>
#include <vector>
#include <glm/glm.hpp>
#include "SpdlogUtils.h"

/**
 * @todo Add code here to pack glyphs for a particular font into a a texture.
 * 
 * - The texture can be resized on a power of 2.
 * - The glyph can be of any font size.
 * 
 * @link https://straypixels.net/texture-packing-for-fonts/ @endlink
 * @link https://blackpawn.com/texts/lightmaps/default.html @endlink
 */

namespace term_engine::utility {
  constexpr size_t INITIAL_TEXTURE_SIZE = 128;
  constexpr unsigned int PADDING = 1;

  class TexturePacker {
  public:
    TexturePacker();

    TexturePacker(const glm::ivec2& initialSize);

    ~TexturePacker();

    glm::ivec2 Insert(unsigned char* buffer_data, const glm::ivec2& size);

    glm::ivec2 GetTextureSize() const;

    const unsigned char* GetBufferData() const;

  private:
    struct TextureNode;

    typedef std::vector<unsigned char> BufferData;
    typedef std::unique_ptr<TextureNode> TextureNodePtr;

    struct TextureNode {
      TextureNode() :
        TextureNode(glm::ivec2(), glm::ivec2(INT_MAX), 0)
      {}

      TextureNode(const glm::ivec2& position, const glm::ivec2& size, const size_t& layer) :
        position_(position),
        size_(size),
        full_(false),
        layer_(layer),
        left_(nullptr),
        right_(nullptr)
      {
        logger->debug("Created node at layer {} with size {},{}", layer_, size_.x, size_.y);
      }

      ~TextureNode()
      {
        logger->debug("Removed node at layer {} with size {},{}", layer_, size_.x, size_.y);

        left_.reset();
        right_.reset();
      }

      glm::ivec2 position_;
      glm::ivec2 size_;
      bool full_;
      size_t layer_;

      TextureNodePtr left_;
      TextureNodePtr right_;
    };

    TextureNodePtr root_;

    glm::ivec2 texture_size_;

    BufferData buffer_;

    bool can_resize_;

    TextureNode* Pack(TextureNode* node, const glm::ivec2& size);

    void ResizeBuffer(const glm::ivec2& new_size);
  };

  
}

#endif // ! GLYPH_PACKING_UTILS_H
