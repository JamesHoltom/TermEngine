/// @author James Holtom

#ifndef GLYPH_PACKING_UTILS_H
#define GLYPH_PACKING_UTILS_H

#include <memory>
#include <vector>
#include <glm/glm.hpp>
#include "SpdlogUtils.h"

namespace term_engine::utility {
  constexpr uint64_t INITIAL_TEXTURE_SIZE = 128;
  constexpr uint32_t PADDING = 1;

  class TexturePacker {
  public:
    TexturePacker();

    TexturePacker(const glm::ivec2& initialSize);

    ~TexturePacker();

    glm::ivec2 Insert(uint8_t* buffer_data, const glm::ivec2& size);

    glm::ivec2 GetTextureSize() const;

    const uint8_t* GetBufferData() const;

  private:
    struct TextureNode;

    typedef std::vector<uint8_t> BufferData;
    typedef std::unique_ptr<TextureNode> TextureNodePtr;

    struct TextureNode {
      TextureNode() :
        TextureNode(glm::ivec2(), glm::ivec2(INT_MAX), 0)
      {}

      TextureNode(const glm::ivec2& position, const glm::ivec2& size, uint64_t layer) :
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
      uint64_t layer_;

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
