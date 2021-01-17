/// @author James Holtom

#ifndef SHADER_H
#define SHADER_H

#include <memory>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>
#include <glm/glm.hpp>

#include "../utility/GLUtils.h"

namespace term_engine::shaders {
  class Shader;

  /// Used to store a list of shader IDs bound to a shader program.
  typedef std::vector<GLuint> StageIDList;
  /// Used to store a list of uniform IDs bound to a shader.
  typedef std::unordered_map<std::string, GLuint> UniformList;
  /// Smart pointer used to share a single instance of a shader.
  typedef std::shared_ptr<Shader> ShaderPtr;
  /// Used to store a list of shaders.
  typedef std::unordered_map<std::string, ShaderPtr> ShaderList;
  /// Iterates over the elements of a _ShaderList_.
  typedef std::pair<std::string, ShaderPtr> ShaderIter;

  /// Wraps an OpenGL shader program.
  class Shader {
  public:
    /// Constructs the shader.
    /**
     * @param[in] name The name of the shader.
     */
    Shader(const std::string& name);

    /// Destroys the shader.
    ~Shader();

    /// Returns the name of the shader.
    /**
     * @returns The name of the shader.
     */
    std::string GetName() const;

    /// Returns the shader program's ID.
    /**
     * @returns The shader program's ID.
     */
    GLuint GetProgramID() const;

    /// Compiles a new shader stage with the given code.
    /**
     * @param[in] source The source code to compile.
     * @param[in] type   The type of stage to create.
     * @see [OpenGL Stages](https://www.khronos.org/opengl/wiki/Shader#Stages)
     * @returns Was the compilation successful?
     */
    bool BuildStageFromString(const GLchar* source, const GLenum& type);

    /// Compiles a new shader stage from the given file.
    /**
     * @param[in] filename The filename to load and compile the code from.
     * @param[in] type     The type of stage to create.
     * @see [OpenGL Stages](https://www.khronos.org/opengl/wiki/Shader#Stages)
     * @returns Was the compilation successful?
     */
    bool BuildStageFromFile(const std::string& filename, const GLenum& type);

    /// Builds the shader program and links any shader stages that have been built.
    /**
     * Also removes the stages from memory afterwards, as they are not required after the program has been built.
     * 
     * @returns Was the build successful?
     */
    bool BuildProgram();

    /// Checks if the shader has been successfully built.
    /**
     * @returns Has the shader been built?
     */
    bool IsBuilt() const;

    /// Tells OpenGL to bind to this shader for rendering.
    void Use() const;

    /// Tells OpenGL to unbind from this shader.
    void Unuse() const;

    /// Sets the integer value(s) of a uniform.
    /**
     * @param[in] name  The name of the uniform to set.
     * @param[in] count The number of values to set.
     * @param[in] data  The values to set.
     */
    void SetUniformInt(const std::string& name, const GLuint& count, const GLint* data);

    /// Sets the unsigned integer value(s) of a uniform.
    /**
     * @param[in] name  The name of the uniform to set.
     * @param[in] count The number of values to set.
     * @param[in] data  The values to set.
     */
    void SetUniformUint(const std::string& name, const GLuint& count, const GLuint* data);

    /// Sets the floating point number value(s) of a uniform.
    /**
     * @param[in] name  The name of the uniform to set.
     * @param[in] count The number of values to set.
     * @param[in] data  The values to set.
     */
    void SetUniformFloat(const std::string& name, const GLuint& count, const GLfloat* data);

    /// Sets the floating point number matrix value of a uniform.
    /**
     * @param[in] name       The name of the uniform to set.
     * @param[in] dimensions The size of the matrix. This can be between a value of 2x2 and 4x4.
     * @param[in] data       The values to set.
     */
    void SetUniformMatrix(const std::string& name, const glm::uvec2& dimensions, const GLfloat* data);

  private:
    /// The name of the shader.
    std::string name_;
    /// The shader program's ID.
    GLuint program_id_;
    /// The list of shaders to compile and build against the program.
    /**
     * @see [OpenGL Shaders](https://www.khronos.org/opengl/wiki/Shader#Stages)
     */
    StageIDList stage_id_list_;

    /// Logs the build log of the program, during _BuildProgram_.
    void PrintProgramLog();

    /// Logs the compilation/linking log of the given shader stage, during either _BuildShaderFromString_ or _BuildShaderFromFile_.
    /**
     * @param[in] stage_id The ID of the shader to log.
     */
    void PrintStageLog(const GLuint& stage_id);

    /// Removes the shader program from memory, if it was built.
    void RemoveProgram();

    /// Removes any compiled shader stages from memory.
    void RemoveShaders();
  };
}

#endif // ! SHADER_H
