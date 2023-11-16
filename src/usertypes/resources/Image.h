/// @author James Holtom

#ifndef IMAGE_H
#define IMAGE_H

#include <filesystem>
#include <memory>
#include "BaseResource.h"
#include "../../rendering/Texture.h"
#include "../../utility/GLUtils.h"

namespace term_engine::usertypes {
  /// @brief The type name for Images.
  constexpr char IMAGE_TYPE[] = "Image";

  /// @brief Stores an image resource, that can be rendered to a game scene.
  class Image : public BaseResource {
  public:
    /// @brief Constructs the image.
    Image(const std::filesystem::path& filepath, rendering::TextureData* data);
    
    /// @brief Destroys the image.
    ~Image();

    /**
     * @brief Returns the type of resource.
     * 
     * @returns The resource type.
     */
    std::string GetResourceType() const;

    /**
     * @brief Returns the size of the image texture.
     * 
     * @returns The texture size, in pixels (px).
     */
    glm::ivec2 GetTextureSize() const;

    /// @brief Binds the image's texture ID to it's index.
    void Use() const;

    /// @brief Updates the debugging information for this resource.
    void UpdateDebugInfo() const;

  protected:
    /// @brief Smart pointer to the texture to render.
    rendering::TexturePtr texture_;
  };

  /**
   * @brief Retrives the image resource with the given filepath. If it's not in the list, it will be loaded.
   * 
   * @param[in] filepath The filepath to the font resource.
   * @returns A raw pointer to the resource, or a null pointer if not found.
   */
  Image* LoadImage(const std::string& filepath);
}

#endif // ! IMAGE_H
