/// @author James Holtom

#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <filesystem>
#include "BaseResource.h"
#include "../../rendering/Buffer.h"
#include "../../rendering/Texture.h"
#include "../../utility/GLUtils.h"

namespace term_engine::usertypes {
  /// @brief The type name for Backgrounds.
  constexpr char BACKGROUND_TYPE[] = "Background";

  /// @brief Stores a background image, that can be rendered to a game scene.
  class Background : public BaseResource {
  public:
    /**
     * @brief Constructs the resource with the given filepath.
     * 
     * @param[in] filepath  The filepath to the resource.
     * @param[in] texture   The texture data for the resource.
     */
    Background(const std::filesystem::path& filepath, const rendering::TextureData& texture);

    /// @brief Destroys the resource.
    ~Background();

    /**
     * @brief Returns the type of resource.
     * 
     * @returns The resource type.
     */
    std::string GetResourceType() const;

    /**
     * @brief Returns the position of the background in the game scene.
     * 
     * @returns The background position, in pixels (px).
     */
    glm::ivec2& GetPosition();

    /**
     * @brief Returns the size of the background in the game scene.
     * 
     * @returns The background size, in pixels (px).
     */
    glm::ivec2& GetSize();

    /**
     * @brief Returns the size of the background texture.
     * 
     * @returns The texture size, in pixels (px).
     */
    glm::ivec2& GetTextureSize();

    /**
     * @brief Returns the colour of the background.
     * 
     * @returns The background colour.
     */
    glm::vec4& GetColour();

    /**
     * @brief Sets the position of the background in the game scene.
     * 
     * @param[in] position The background position, in pixels (px).
     */
    void SetPosition(const glm::ivec2& position);

    /**
     * @brief Sets the size of the background in the game scene.
     * 
     * @param[in] size The background size, in pixels (px).
     */
    void SetSize(const glm::ivec2& size);

    /**
     * @brief Sets the colour of the background.
     * 
     * @param[in] colour The colour of the background.
     */
    void SetColour(const glm::vec4& colour);

    /// @brief Resets the position, size and colour of the background.
    void Reset();

    /**
     * @brief Copys the background vertex data into a buffer.
     * 
     * @param[in] buffer The buffer to copy data to.
     */
    void CopyToBuffer(rendering::Buffer& buffer) const;

    /// @brief Binds the background's texture ID to it's index.
    void Use() const;

    /// @brief Updates the debugging information for this resource.
    void UpdateDebugInfo() const;

  protected:
    /// @brief The texture ID and size.
    rendering::TextureData texture_;
    /// @brief The position of the background in the game scene, in pixels (px).
    glm::ivec2 position_;
    /// @brief The size of the background in the game scene, in pixels (px).
    glm::ivec2 size_;
    /// @brief The colour of the background.
    glm::vec4 colour_;
  };

  /**
   * @brief Adds a background resource to the list of resources.
   * 
   * @param[in] filepath  The filepath to the background resource.
   * @returns A raw pointer to the resource.
   */
  Background* LoadBackground(const std::string& filepath);
}

#endif // ! BACKGROUND_H
