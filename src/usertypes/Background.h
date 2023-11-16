/// @author James Holtom

#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <filesystem>
#include <memory>
#include <glm/glm.hpp>
#include "resources/Image.h"
#include "../rendering/Buffer.h"

namespace term_engine::usertypes {
  /// @brief Handles the rendering of background images to a game scene.
  class Background {
  public:
    /// @brief Constructs the background.
    Background();
    
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
     * @brief Returns the colour of the background.
     * 
     * @returns The background colour.
     */
    glm::vec4& GetColour();

    /**
     * @brief Returns the filepath of the background texture.
     * 
     * @returns The filepath of the background texture.
     */
    std::string GetSource() const;

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

    /**
     * @brief Loads a new background texture with the given filepath.
     * 
     * @param[in] filepath The filepath to the new texture.
     */
    void SetSource(const std::string& filepath);

    /// @brief Unsets the background texture and resets the position/size.
    void RemoveSource();

    /**
     * @brief Returns if a background texture is loaded.
     * 
     * @returns If a texture is loaded.
     */
    bool IsLoaded() const;

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
    /// @brief Raw pointer to the texture to render.
    Image* image_;
    /// @brief The position of the background in the game scene, in pixels (px).
    glm::ivec2 position_;
    /// @brief The size of the background in the game scene, in pixels (px).
    glm::ivec2 size_;
    /// @brief The colour of the background.
    glm::vec4 colour_;
  };
}

#endif // ! BACKGROUND_H
