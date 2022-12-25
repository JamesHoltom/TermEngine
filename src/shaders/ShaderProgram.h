/// @author James Holtom

#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <memory>
#include <string>
#include <vector>
#include "../utility/GLUtils.h"
#include "../utility/SolUtils.h"

namespace shaders {
  class ShaderProgram;

  typedef std::shared_ptr<ShaderProgram> ShaderProgramPtr;
  typedef std::vector<ShaderProgramPtr> ShaderProgramList;

  class ShaderProgram
  {
  public:
    ShaderProgram();
    ~ShaderProgram();

    GLuint GetProgramId() const;

    void AttachFile(const std::string& filepath, const GLenum& type);

    void AttachString(const std::string& glsl_code, const GLenum& type);

    void Link();

    void Use() const;

    static GLuint GetProgramInUse();

    /**
     * @brief Adds a shader program to the list.
     * 
     * @returns A smart pointer to the shader program if it was added to the list, or a null pointer if it failed.
     */
    static ShaderProgramPtr& Add();

    /**
     * @brief Adds a shader program to the list, using Lua's colon syntax (i.e. ShaderProgram:new(...)).
     * 
     * @param[in] self  Reference to the shader program. Unused.
     * @returns A smart pointer to the shader program if it was added to the list, or a null pointer if it failed.
     */
    static ShaderProgramPtr& AddSelf(sol::object self);

    /**
     * @brief Removes a shader program from the list.
     * 
     * @param[in] obj A smart pointer to the shader program.
     */
    static void Remove(ShaderProgramPtr& obj);

    /**
     * @brief Retrieves the list of shader programs.
     * 
     * @returns The list of shader programs.
     */
    static ShaderProgramList& GetList();

    /**
     * @brief Returns the number of shader programs.
     * 
     * @returns The number of shader programs.
     */
    static int Count();
    
    /// @brief Removes all shader programs in the list.
    static void CleanUp();

  protected:
    void RemoveCompiledShaders();

    GLuint program_id_;
    bool linked_;
    GLuint shader_vert_id_;
    GLuint shader_frag_id_;
    GLuint shader_geom_id_;
    static GLuint program_in_use_;
    static ShaderProgramList shader_list_;
  };
}

#endif // ! SHADER_PROGRAM_H