#include "TexturePacker.h"

namespace term_engine::utility {
  TexturePacker::TexturePacker() :
    TexturePacker(glm::ivec2(INITIAL_TEXTURE_SIZE))
  {
    can_resize_ = true;
  }

  TexturePacker::TexturePacker(const glm::ivec2& initialSize) :
    texture_size_(initialSize),
    buffer_(initialSize.x * initialSize.y, 0)
  {
    can_resize_ = false;
    root_ = std::make_unique<TextureNode>();
  }

  TexturePacker::~TexturePacker()
  {
    root_.reset();
    root_ = nullptr;
    buffer_.clear();
  }

  glm::ivec2 TexturePacker::Insert(unsigned char* buffer_data, const glm::ivec2& size)
  {
    TextureNode* node = Pack(root_.get(), size);

    if (node == nullptr)
    {
      if (can_resize_)
      {
        ResizeBuffer(texture_size_ * 2);
        node = Pack(root_.get(), size);
      }
      
      if (node == nullptr)
      {
        logger->error("Cannot fit texture of size {},{} into packed texture of size {},{}!", size.x, size.y, texture_size_.x, texture_size_.y);

        return glm::ivec2();
      }
    }

    if (buffer_data)
    {
      for (size_t y = 0; y < node->size_.y; ++y)
      {
        for (size_t x = 0; x < node->size_.x; ++x)
        {
          const unsigned int setX = node->position_.x + x;
          const unsigned int setY = node->position_.y + y;

          buffer_.at((setY * texture_size_.x) + setX) = buffer_data[(y * size.x) + x];
        }
      }
    }

    return node->position_;
  }

  glm::ivec2 TexturePacker::GetTextureSize() const
  {
    return texture_size_;
  }

  const unsigned char* TexturePacker::GetBufferData() const
  {
    return buffer_.data();
  }

  TexturePacker::TextureNode* TexturePacker::Pack(TextureNode* node, const glm::ivec2& size)
  {
    // If the node is fully packed, the new node isn't going to fit here.
    if (node->full_)
    {
      return nullptr;
    }
    // If this is a non-leaf, try inserting into the left node, then the right node if the left is full.
    else if (node->left_ && node->right_)
    {
      TextureNode* leftVal = Pack(node->left_.get(), size);

      if (leftVal != nullptr)
      {
        return leftVal;
      }

      return Pack(node->right_.get(), size);
    }
    // If this is a empty leaf, try to fill it.
    else
    {
      glm::ivec2 real_size = node->size_;

      if (node->position_.x + node->size_.x == INT_MAX)
      {
        real_size.x = texture_size_.x - node->position_.x;
      }

      if (node->position_.y + node->size_.y == INT_MAX)
      {
        real_size.y = texture_size_.y - node->position_.y;
      }

      // If the size of the texture perfectly fits the node, fill it and return.
      if (node->size_ == size)
      {
        node->full_ = true;

        return node;
      }
      // If the size of the node isn't big enough to hold the texture, return.
      else if (glm::any(glm::lessThan(real_size, size)))
      {
        return nullptr;
      }
      else
      {
        TextureNode* left;
        TextureNode* right;
        glm::ivec2 remainder = real_size - size;
        bool is_vertical_split = remainder.x < remainder.y;

        if (glm::all(glm::equal(remainder, glm::ivec2(0))))
        {
          is_vertical_split = node->size_.x < node->size_.y;
        }

        if (is_vertical_split)
        {
          left = new TextureNode(node->position_, glm::ivec2(node->size_.x, size.y), node->layer_ + 1);
          right = new TextureNode(glm::ivec2(node->position_.x, node->position_.y + size.y), glm::ivec2(node->size_.x, node->size_.y - size.y), node->layer_ + 1);
        }
        else
        {
          left = new TextureNode(node->position_, glm::ivec2(size.x, node->size_.y), node->layer_ + 1);
          right = new TextureNode(glm::ivec2(node->position_.x + size.x, node->position_.y), glm::ivec2(node->size_.x - size.x, node->size_.y), node->layer_ + 1);
        }

        node->left_ = std::unique_ptr<TextureNode>(left);
        node->right_ = std::unique_ptr<TextureNode>(right);

        return Pack(node->left_.get(), size);
      }
    }
  }

  void TexturePacker::ResizeBuffer(const glm::ivec2& new_size)
  {
    BufferData new_data(new_size.x * new_size.y, 0);

    for (size_t y = 0; y < texture_size_.y; ++y)
    {
      for (size_t x = 0; x < texture_size_.x; ++x)
      {
        new_data[(y * new_size.x) + x] = buffer_[(y * texture_size_.x) + x];
      }
    }

    texture_size_ = new_size;
    buffer_ = std::move(new_data);
  }
}