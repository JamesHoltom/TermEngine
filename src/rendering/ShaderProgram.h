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
    int location_;
    std::string name_;
    int count_;
    uint32_t type_;
  };

  class ShaderProgram
  {
  public:
    ShaderProgram();
    ~ShaderProgram();

    uint32_t GetProgramId() const;

    bool IsLinked() const;

    void AttachFile(const std::filesystem::path& filepath, uint32_t type);

    void AttachString(const std::string& glsl_code, uint32_t type);

    void Link();

    void Use() const;

    template <typename T> void SetUniform(const std::string& name, T data) const;

    template <typename T> void SetUniformVector(const std::string& name, const T& data) const;
    
    template <typename T> void SetUniformMatrix(const std::string& name, const T& data, uint8_t transpose = GL_FALSE) const;

    //template <typename T> void SetUniformArray(const std::string& name, const GLsizei& count, const T* data) const;

  private:
    uint32_t program_id_;
    bool is_linked_;
    uint32_t shader_vert_id_;
    uint32_t shader_frag_id_;
    uint32_t shader_geom_id_;
    UniformList uniforms_;

    void RemoveCompiledShaders();
  };
}

#endif // ! SHADER_PROGRAM_H