/// @author James Holtom

#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <filesystem>
#include "Buffer.h"
#include "Texture.h"
#include "../utility/GLUtils.h"

namespace term_engine::rendering {
  /// @brief Manages a background image that can be rendered to a game scene.
  class Background {
  public:
    /// @brief Constructs the background.
    Background();

    /**
     * @brief Constructs the background with the given filename.
     * 
     * @param[in] filepath The path to the image file.
     */
    Background(const std::filesystem::path& filepath);

    /// @brief Destroys the background.
    ~Background();

    /**
     * @brief Returns the position of the background in the game scene.
     * 
     * @returns The background position, in pixels (px).
     */
    glm::ivec2 GetPosition() const;

    /**
     * @brief Returns the size of the background in the game scene.
     * 
     * @returns The background size, in pixels (px).
     */
    glm::ivec2 GetSize() const;

    /**
     * @brief Returns the size of the background texture.
     * 
     * @returns The texture size, in pixels (px).
     */
    glm::ivec2 GetTextureSize() const;

    /**
     * @brief Returns the colour of the background.
     * 
     * @returns The background colour.
     */
    glm::vec4 GetColour() const;

    /**
     * @brief Returns if the background has a loaded image.
     * 
     * @returns Is the background loaded?
     */
    bool IsLoaded() const;

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
     * @brief Loads the given image file as a background texture.
     * @note If the load fails, the old texture is retained.s
     * 
     * @param[in] filepath The image file to load.
     */
    void Load(const std::string& filepath);

    /// @brief Unloads the current background texture, if one is loaded.
    void Unload();

    /**
     * @brief Copys the background vertex data into a buffer.
     * 
     * @param[in] buffer The buffer to copy data to.
     */
    void CopyToBuffer(rendering::Buffer& buffer) const;

    /// @brief Binds the background's texture ID to it's index.
    void Use() const;

  private:
    /// @brief The path to the file containing the texture.
    std::filesystem::path filepath_;
    /// @brief The texture ID and size.
    rendering::TextureData texture_;
    /// @brief Is a background currently loaded?
    bool is_loaded_;
    /// @brief The position of the background in the game scene, in pixels (px).
    glm::ivec2 position_;
    /// @brief The size of the background in the game scene, in pixels (px).
    glm::ivec2 size_;
    /// @brief The colour of the background.
    glm::vec4 colour_;
  };
}

#endif // ! BACKGROUND_H
