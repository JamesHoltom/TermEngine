#include "Background.h"
#include "../system/FileFunctions.h"
#include "../utility/DebugUtils.h"
#include "../utility/SpdlogUtils.h"

namespace term_engine::rendering {
  Background::Background() :
    filepath_(""),
    is_loaded_(false),
    position_(glm::ivec2()),
    size_(glm::ivec2()),
    colour_(glm::vec4(255.0f))
  {
    utility::logger->debug("Created background with no loaded image.");
  }

  Background::Background(const std::filesystem::path& filepath) :
    filepath_(filepath),
    position_(glm::ivec2()),
    colour_(glm::vec4(255.0f))
  {
    texture_ = CreateTextureFromImage(filepath, 1);
    is_loaded_ = texture_.texture_id_ > 0;
    size_ = texture_.size_;

    if (is_loaded_)
    {
      utility::logger->debug("Created background with {}x{} image \"{}\" at texture ID {}.", size_.x, size_.y, filepath.string(), texture_.texture_id_);
    }
    else
    {
      utility::logger->error("Created background, but failed to load image \"{}\".", filepath.string());
    }
  }

  Background::~Background()
  {
    if (is_loaded_)
    {
      DeleteTexture(texture_);
    }

    utility::logger->debug("Destroyed background with image \"{}\".", filepath_.string());
  }

  glm::ivec2 Background::GetPosition() const
  {
    return position_;
  }

  glm::ivec2 Background::GetSize() const
  {
    return size_;
  }

  glm::ivec2 Background::GetTextureSize() const
  {
    return texture_.size_;
  }

  glm::vec4 Background::GetColour() const
  {
    return colour_;
  }

  bool Background::IsLoaded() const
  {
    return is_loaded_;
  }

  void Background::SetPosition(const glm::ivec2& position)
  {
    position_ = position;
  }

  void Background::SetSize(const glm::ivec2& size)
  {
    size_ = size;
  }

  void Background::SetColour(const glm::vec4& colour)
  {
    colour_ = colour;
  }

  void Background::Reset()
  {
    position_ = glm::ivec2();
    size_ = texture_.size_;
    colour_ = glm::vec4(255.0);
  }

  void Background::Load(const std::string& filepath)
  {
    std::filesystem::path find_path = system::SearchForResourcePath(filepath);

    if (find_path == "")
    {
      return;
    }

    TextureData new_texture = CreateTextureFromImage(find_path, 1);

    if (new_texture.texture_id_ > 0)
    {
      Unload();

      texture_ = new_texture;
      is_loaded_ = true;
      size_ = texture_.size_;
    }
  }

  void Background::Unload()
  {
    DeleteTexture(texture_);

    Reset();

    texture_ = TextureData();
    is_loaded_ = false;
  }

  void Background::CopyToBuffer(rendering::Buffer& buffer) const
  {
    if (is_loaded_)
    {
      /* Draw order:
      * 1
      * |\
      * | \
      * 3--2
      */
      buffer.data.push_back(rendering::BufferData(glm::vec2(position_), glm::vec2(), colour_));
      buffer.data.push_back(rendering::BufferData(glm::vec2(position_ + size_), glm::vec2(1.0f), colour_));
      buffer.data.push_back(rendering::BufferData(glm::vec2(position_.x, position_.y + size_.y), glm::vec2(0.0f, 1.0f), colour_));

      /* Draw order:
      * 1--2
      *  \ |
      *   \|
      *    3
      */
      buffer.data.push_back(rendering::BufferData(glm::vec2(position_), glm::vec2(), colour_));
      buffer.data.push_back(rendering::BufferData(glm::vec2(position_.x + size_.x, position_.y), glm::vec2(1.0f, 0.0f), colour_));
      buffer.data.push_back(rendering::BufferData(glm::vec2(position_ + size_), glm::vec2(1.0f), colour_));
    }
  }

  void Background::Use() const
  {
    glActiveTexture(GL_TEXTURE0 + texture_.texture_index_);
    glBindTexture(GL_TEXTURE_2D, texture_.texture_id_);
  }
}
