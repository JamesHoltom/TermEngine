/// @author James Holtom

#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <string>
#include <vector>
#include "../utility/GLUtils.h"
#include "../utility/SolUtils.h"

namespace term_engine::rendering {
  struct Uniform;

  typedef std::vector<Uniform> UniformList;

  struct Uniform {
    GLint location_;
    std::string name_;
    GLint count_;
    GLenum type_;
  };

  class ShaderProgram
  {
  public:
    ShaderProgram();
    ~ShaderProgram();

    GLuint GetProgramId() const;

    bool IsLinked() const;

    void AttachFile(const std::filesystem::path& filepath, const GLenum& type);

    void AttachString(const std::string& glsl_code, const GLenum& type);

    void Link();

    void Use() const;

    template <typename T> void SetUniform(const std::string& name, const T& data) const;
    
    template <typename T> void SetUniformMatrix(const std::string& name, const T& data, const GLboolean& transpose = GL_FALSE) const;

    // TODO: Figure out how to implement array uniforms.
    //template <typename T> void SetUniformArray(const std::string& name, const GLsizei& count, const T* data) const;

  private:
    void RemoveCompiledShaders();

    GLuint program_id_;
    bool is_linked_;
    GLuint shader_vert_id_;
    GLuint shader_frag_id_;
    GLuint shader_geom_id_;
    UniformList uniforms_;
  };
}

#endif // ! SHADER_PROGRAM_H