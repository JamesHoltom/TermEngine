/// @author James Holtom

#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include "BaseResource.h"
#include "../../utility/GLUtils.h"
#include "../../utility/SolUtils.h"

namespace term_engine::usertypes {
  class ShaderProgram;

  /// @brief The type name for ShaderProgram.
  constexpr char SHADER_PROGRAM_TYPE[] = "ShaderProgram";
  constexpr char DEFAULT_TEXT_SHADER[] = "text_default";
  constexpr char DEFAULT_BG_SHADER[] = "bg_default";

  /// @brief Wraps around OpenGL shaders and uniforms to draw content to a window.
  class ShaderProgram : public BaseResource {
  public:
    /**
     * @brief Constructs the resource with the given name.
     * 
     * @param[in] name        The name of the resource.
     * @param[in] program_id  The OpenGL program ID for the shader resource.
     * @param[in] uniforms    The list of shader uniforms for the shader resource.
     */
    ShaderProgram(const std::string& name, uint32_t program_id, const utility::UniformList& uniforms);

    /// @brief Destroys the resource.
    ~ShaderProgram();

    /**
     * @brief Returns the type of resource.
     * 
     * @returns The resource type.
     */
    std::string GetResourceType() const;

    /**
     * @brief Returns the ID of the shader program.
     * 
     * @returns The shader program ID.
     */
    uint32_t GetProgramId() const;

    /// @brief Binds the shader program.
    void Use() const;

    /**
     * @brief Sets a scalar uniform with the given value.
     * 
     * @tparam T The scalar type of uniform being set.
     * @param[in] name  The name of the uniform. It must exist within the GLSL code.
     * @param[in] data  The data to set.
     */
    template <typename T> void SetUniform(const std::string& name, T data) const;

    /**
     * @brief Sets a vector uniform with the given value.
     * 
     * @tparam T The GLM vector type of uniform being set.
     * @param[in] name  The name of the uniform. It must exist within the GLSL code.
     * @param[in] data  The data to set.
     */
    template <typename T> void SetUniformVector(const std::string& name, const T& data) const;
    
    /**
     * @brief Sets a matrix type uniform with the given value.
     * 
     * @tparam T The GLM matrix type of uniform being set.
     * @param[in] name      The name of the uniform. It must exist within the GLSL code.
     * @param[in] data      The data to set.
     * @param[in] transpose Whether to transpose the matrix (i.e. from row-major to column-major and vice versa).
     */
    template <typename T> void SetUniformMatrix(const std::string& name, const T& data, uint8_t transpose = GL_FALSE) const;

    /// @todo Add the option to set array uniforms.
    //template <typename T> void SetUniformArray(const std::string& name, const GLsizei& count, const T* data) const;

    /// @brief Updates the debugging information for this shader.
    void UpdateDebugInfo() const;

  private:
    /// @brief The shader program ID.
    uint32_t program_id_;
    /// @brief The list of uniforms present within the shader program.
    utility::UniformList uniforms_;
  };

  void InitDefaultShaders();

  ShaderProgram* AddShader(const std::string& name, const std::string& vert_code, const std::string& frag_code, const std::string& geom_code);

  ShaderProgram* GetShader(const std::string& name);
}

#endif // ! SHADER_PROGRAM_H