/// @author James Holtom

#ifndef GLYPH_PACKING_UTILS_H
#define GLYPH_PACKING_UTILS_H

#include <memory>
#include <vector>
#include <glm/glm.hpp>
#include "../utility/LogUtils.h"

namespace term_engine::rendering {
  struct TextureNode;
  
  /// @brief Used to store texture buffer data.
  typedef std::vector<uint8_t> TextureBufferData;
  /// @brief Unique pointer to a texture packing node.
  typedef std::unique_ptr<TextureNode> TextureNodePtr;

  /// @brief The initial size of the texture when creating a texture packer.
  constexpr uint64_t INITIAL_TEXTURE_SIZE = 128;
  /// @brief The amount of space to pad each node with, in pixels (px).
  constexpr uint32_t PADDING = 1;

  /// @brief Represents a section of the texture.
  struct TextureNode {
    /**
     * @brief Constructs the node with the given parameters.
     * 
     * @param[in] position  The position of the node.
     * @param[in] size      The size of the node.
     * @param[in] layer     The layer of the node.
     */
    TextureNode(const glm::ivec2& position, const glm::ivec2& size, uint64_t layer) :
      position_(position),
      size_(size),
      full_(false),
      layer_(layer),
      left_(nullptr),
      right_(nullptr)
    {
      utility::LogDebug("Created node at layer {} with size {},{}", layer_, size_.x, size_.y);
    }

    /// @brief Destroys the node.
    ~TextureNode()
    {
      utility::LogDebug("Removed node at layer {} with size {},{}", layer_, size_.x, size_.y);

      left_.reset();
      right_.reset();
    }

    /// @brief The position of the node, in pixels (px).
    glm::ivec2 position_;
    /// @brief The size of the node, in pixels (px).
    glm::ivec2 size_;
    /// @brief Used to check if the node is full, and cannot be split.
    bool full_;
    /// @brief The layer of the node.
    uint64_t layer_;
    /// @brief The left/top node on the next layer down, if set.
    TextureNodePtr left_;
    /// @brief The right/bottom node on the next layer down, if set.
    TextureNodePtr right_;
  };

  /// @brief Used to pack images into 1 texture.
  class TexturePacker {
  public:
    /// @brief Constructs the texture packer.
    TexturePacker();

    /**
     * @brief Constructs the texture packer with the given size.
     * 
     * @param[in] initialSize The initial size to set the texture to.
     */
    TexturePacker(const glm::ivec2& initialSize);

    /// @brief Destroys the texture packer.
    ~TexturePacker();

    /**
     * @brief Inserts image data into a node.
     * 
     * @param[in] buffer_data The image data to insert.
     * @param[in] size        The size of the image.
     * @returns The position of the inserted node in the texture, in pixels (px).
     */
    glm::ivec2 Insert(uint8_t* buffer_data, const glm::ivec2& size);

    /**
     * @brief Returns the size of the texture.
     * 
     * @returns The size of the texture.
     */
    glm::ivec2 GetTextureSize() const;

    /**
     * @brief Returns the texture data.
     * 
     * @returns The texture data.
     */
    const uint8_t* GetTextureData() const;

  private:
    /// @brief The root-level node.
    TextureNodePtr root_;
    /// @brief The size of the texture.
    glm::ivec2 texture_size_;
    /// @brief The texture data.
    TextureBufferData buffer_;
    /// @brief Can the texture be resized? If an initial value is set, or if the maximum size is reached, this is set to false.
    bool can_resize_;

    /**
     * @brief Attempts to find space in the given node for the specified size.
     * @see https://blackpawn.com/texts/lightmaps/default.html
     * 
     * @param[in,out] node The node to check for space.
     * @param[in] size The size of an image to pack.
     * @returns A raw pointer to either the node's left/top or right/bottom split which can accept the given size, or a null pointer if it cannot.
     */
    TextureNode* Pack(TextureNode* node, const glm::ivec2& size);

    /**
     * @brief Increases the size of the texture data.
     * 
     * @param[in] new_size The new size of the texture data.
     */
    void ResizeBuffer(const glm::ivec2& new_size);
  };
}

#endif // ! GLYPH_PACKING_UTILS_H
