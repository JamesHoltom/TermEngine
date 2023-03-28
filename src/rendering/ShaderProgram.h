/// @author James Holtom

#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <string>
#include <vector>
#include "../utility/GLUtils.h"
#include "../utility/SolUtils.h"

namespace term_engine::rendering {
  struct Uniform;

  /// @brief Used to store a list of uniforms for a shader.
  typedef std::vector<Uniform> UniformList;

  /// @brief Defines an OpenGL shader uniform, and is created when a shader is compiled.
  struct Uniform {
    /// @brief The location index of the uniform.
    int location_;
    /// @brief The name of the uniform.
    std::string name_;
    /// @brief The number of values this uniform holds.
    int count_;
    /// @brief The data type of the uniform.
    uint32_t type_;
  };

  /// @brief 
  class ShaderProgram {
  public:
    /// @brief Constructs the shader program.
    ShaderProgram();

    /// @brief Destroys the shader program.
    ~ShaderProgram();

    /**
     * @brief Returns the ID of the shader program.
     * 
     * @returns The shader program ID.
     */
    uint32_t GetProgramId() const;

    /**
     * @brief Returns if the shader program has been fully compiled and linked.
     * 
     * @returns If the shader program is linked.
     */
    bool IsLinked() const;

    /**
     * @brief Loads a file containing GLSL code and compiles it into a shader stage.
     * 
     * @param[in] filepath  The filepath to the GLSL code to compile.
     * @param[in] type      The type of the shader stage.
     */
    void AttachFile(const std::filesystem::path& filepath, uint32_t type);

    /**
     * @brief Compiles a string of GLSL code into a shader stage.
     * 
     * @param[in] glsl_code The GLSL code to compile.
     * @param[in] type      The type of the shader stage.
     */
    void AttachString(const std::string& glsl_code, uint32_t type);

    /// @brief Links the compiled stages to the shader program.
    void Link();

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

  private:
    /// @brief The shader program ID.
    uint32_t program_id_;
    /// @brief Flag to determine if the shader program has compiled stages and is linked.
    bool is_linked_;
    /// @brief The ID of the vertex shader stage, if set.
    uint32_t shader_vert_id_;
    /// @brief The ID of the fragment shader stage, if set.
    uint32_t shader_frag_id_;
    /// @brief The ID of the geometry shader stage, if set.
    uint32_t shader_geom_id_;
    /// @brief The list of uniforms present within the shader program.
    UniformList uniforms_;

    /// @brief Removes all compiled shader stages, after linking the program.
    void RemoveCompiledShaders();
  };
}

#endif // ! SHADER_PROGRAM_H