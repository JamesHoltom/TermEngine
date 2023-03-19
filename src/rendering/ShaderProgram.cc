#include <filesystem>
#include "ShaderProgram.h"
#include "../system/FileFunctions.h"
#include "../utility/GLUtils.h"
#include "../utility/SpdlogUtils.h"

namespace term_engine::rendering {
  ShaderProgram::ShaderProgram() :
    is_linked_(false),
    shader_vert_id_(0),
    shader_frag_id_(0),
    shader_geom_id_(0)
  {
    program_id_ = glCreateProgram();

    utility::logger->debug("Created shader program with ID {}.", program_id_);
  }

  ShaderProgram::~ShaderProgram()
  {
    RemoveCompiledShaders();

    utility::logger->debug("Removed shader program with ID {}.", program_id_);

    glDeleteProgram(program_id_);
  }

  uint32_t ShaderProgram::GetProgramId() const
  {
    return program_id_;
  }

  bool ShaderProgram::IsLinked() const
  {
    return is_linked_;
  }

  void ShaderProgram::AttachFile(const std::filesystem::path& filepath, uint32_t type)
  {
    const std::string glsl_code = system::ReadFile(filepath);

    AttachString(glsl_code, type);
  }

  void ShaderProgram::AttachString(const std::string& glsl_code, uint32_t type)
  {
    if (glsl_code != "")
    {
      utility::ShaderProcessResult results = utility::CompileShaderStage(glsl_code, type);

      if (results.success_ == GL_TRUE)
      {
        switch (type)
        {
          case GL_VERTEX_SHADER:    shader_vert_id_ = results.id_;  break;
          case GL_FRAGMENT_SHADER:  shader_frag_id_ = results.id_;  break;
          case GL_GEOMETRY_SHADER:  shader_geom_id_ = results.id_;  break;
        }

        glAttachShader(program_id_, results.id_);

        utility::logger->debug("Compiled {} shader with ID {}.", utility::GetShaderTypeName(type), results.id_);
      }
      else
      {
        utility::logger->error("Failed to compile {} shader with ID {}.", utility::GetShaderTypeName(type), results.id_);
      }
    }
    else
    {
      utility::logger->warn("No GLSL code to compile!");
    }
  }

  void ShaderProgram::Link()
  {
    utility::ShaderProcessResult results = utility::LinkShaderProgram(program_id_);

    if (results.success_ == GL_TRUE)
    {
      RemoveCompiledShaders();

      is_linked_ = true;

      int count, max_len;

      glGetProgramiv(program_id_, GL_ACTIVE_UNIFORMS, &count);
      glGetProgramiv(program_id_, GL_ACTIVE_UNIFORM_MAX_LENGTH, &max_len);

      uniforms_.reserve(count);

      for (int i = 0; i < count; ++i)
      {
        Uniform uniform;
        char* uniform_name = new char[max_len];
        int name_len;

        glGetActiveUniform(program_id_, i, max_len, &name_len, &uniform.count_, &uniform.type_, uniform_name);

        uniform.location_ = i;
        uniform.name_ = std::string(uniform_name, name_len);

        delete[] uniform_name;

        uniforms_.push_back(uniform);

        utility::logger->debug("Found uniform #{} with name: \"{}\", count: {}, type: {}", uniform.location_, uniform.name_, uniform.count_, uniform.type_);
      }

      utility::logger->debug("Linked shader program with ID {}.", program_id_);
    }
    else
    {
      utility::logger->debug("Failed to link shader program with ID {}.", program_id_);
    }
  }

  void ShaderProgram::Use() const
  {
    glUseProgram(program_id_);
  }

  void ShaderProgram::RemoveCompiledShaders()
  {
    if (glIsShader(shader_vert_id_))
    {
      glDetachShader(program_id_, shader_vert_id_);
      glDeleteShader(shader_vert_id_);
      shader_vert_id_ = 0;
    }

    if (glIsShader(shader_frag_id_))
    {
      glDetachShader(program_id_, shader_frag_id_);
      glDeleteShader(shader_frag_id_);
      shader_frag_id_ = 0;
    }

    if (glIsShader(shader_geom_id_))
    {
      glDetachShader(program_id_, shader_geom_id_);
      glDeleteShader(shader_geom_id_);
      shader_geom_id_ = 0;
    }
  }

  template<>
  void ShaderProgram::SetUniform(const std::string& name, float data) const
  {
    if (!is_linked_)
    {
      utility::logger->warn("Cannot set uniform for an unlinked shader.");

      return;
    }

    Use();

    const int location = glGetUniformLocation(program_id_, name.c_str());

    if (location == -1)
    {
      utility::logger->warn("Uniform \"{}\" does not exist.", name);

      return;
    }

    glUniform1f(location, data);
  }

  template<>
  void ShaderProgram::SetUniform(const std::string& name, int data) const
  {
    if (!is_linked_)
    {
      utility::logger->warn("Cannot set uniform for an unlinked shader.");

      return;
    }

    Use();

    const int location = glGetUniformLocation(program_id_, name.c_str());

    if (location == -1)
    {
      utility::logger->warn("Uniform \"{}\" does not exist.", name);

      return;
    }

    if (data >= 0)
    {
      glUniform1i(location, data);
    }
    else
    {
      glUniform1ui(location, data);
    }
  }

  template<>
  void ShaderProgram::SetUniformVector(const std::string& name, const glm::vec2& data) const
  {
    if (!is_linked_)
    {
      utility::logger->warn("Cannot set uniform for an unlinked shader.");

      return;
    }

    Use();

    const int location = glGetUniformLocation(program_id_, name.c_str());

    if (location == -1)
    {
      utility::logger->warn("Uniform \"{}\" does not exist.", name);

      return;
    }

    glUniform2fv(location, 1, glm::value_ptr(data));
  }

  template<>
  void ShaderProgram::SetUniformVector(const std::string& name, const glm::ivec2& data) const
  {
    if (!is_linked_)
    {
      utility::logger->warn("Cannot set uniform for an unlinked shader.");

      return;
    }

    Use();

    const int location = glGetUniformLocation(program_id_, name.c_str());

    if (location == -1)
    {
      utility::logger->warn("Uniform \"{}\" does not exist.", name);

      return;
    }

    glUniform2iv(location, 1, glm::value_ptr(data));
  }

  template<>
  void ShaderProgram::SetUniformVector(const std::string& name, const glm::uvec2& data) const
  {
    if (!is_linked_)
    {
      utility::logger->warn("Cannot set uniform for an unlinked shader.");

      return;
    }

    Use();

    const int location = glGetUniformLocation(program_id_, name.c_str());

    if (location == -1)
    {
      utility::logger->warn("Uniform \"{}\" does not exist.", name);

      return;
    }

    glUniform2uiv(location, 1, glm::value_ptr(data));
  }

  template<>
  void ShaderProgram::SetUniformVector(const std::string& name, const glm::vec3& data) const
  {
    if (!is_linked_)
    {
      utility::logger->warn("Cannot set uniform for an unlinked shader.");

      return;
    }

    Use();

    const int location = glGetUniformLocation(program_id_, name.c_str());

    if (location == -1)
    {
      utility::logger->warn("Uniform \"{}\" does not exist.", name);

      return;
    }

    glUniform3fv(location, 1, glm::value_ptr(data));
  }

  template<>
  void ShaderProgram::SetUniformVector(const std::string& name, const glm::ivec3& data) const
  {
    if (!is_linked_)
    {
      utility::logger->warn("Cannot set uniform for an unlinked shader.");

      return;
    }

    Use();

    const int location = glGetUniformLocation(program_id_, name.c_str());

    if (location == -1)
    {
      utility::logger->warn("Uniform \"{}\" does not exist.", name);

      return;
    }

    glUniform3iv(location, 1, glm::value_ptr(data));
  }

  template<>
  void ShaderProgram::SetUniformVector(const std::string& name, const glm::uvec3& data) const
  {
    if (!is_linked_)
    {
      utility::logger->warn("Cannot set uniform for an unlinked shader.");

      return;
    }

    Use();

    const int location = glGetUniformLocation(program_id_, name.c_str());

    if (location == -1)
    {
      utility::logger->warn("Uniform \"{}\" does not exist.", name);

      return;
    }

    glUniform3uiv(location, 1, glm::value_ptr(data));
  }

  template<>
  void ShaderProgram::SetUniformVector(const std::string& name, const glm::vec4& data) const
  {
    if (!is_linked_)
    {
      utility::logger->warn("Cannot set uniform for an unlinked shader.");

      return;
    }

    Use();

    const int location = glGetUniformLocation(program_id_, name.c_str());

    if (location == -1)
    {
      utility::logger->warn("Uniform \"{}\" does not exist.", name);

      return;
    }

    glUniform4fv(location, 1, glm::value_ptr(data));
  }

  template<>
  void ShaderProgram::SetUniformVector(const std::string& name, const glm::ivec4& data) const
  {
    if (!is_linked_)
    {
      utility::logger->warn("Cannot set uniform for an unlinked shader.");

      return;
    }

    Use();

    const int location = glGetUniformLocation(program_id_, name.c_str());

    if (location == -1)
    {
      utility::logger->warn("Uniform \"{}\" does not exist.", name);

      return;
    }

    glUniform4iv(location, 1, glm::value_ptr(data));
  }

  template<>
  void ShaderProgram::SetUniformVector(const std::string& name, const glm::uvec4& data) const
  {
    if (!is_linked_)
    {
      utility::logger->warn("Cannot set uniform for an unlinked shader.");

      return;
    }

    Use();

    const int location = glGetUniformLocation(program_id_, name.c_str());

    if (location == -1)
    {
      utility::logger->warn("Uniform \"{}\" does not exist.", name);

      return;
    }

    glUniform4uiv(location, 1, glm::value_ptr(data));
  }

  template<>
  void ShaderProgram::SetUniformMatrix(const std::string& name, const glm::mat2& data, uint8_t transpose) const
  {
    if (!is_linked_)
    {
      utility::logger->warn("Cannot set uniform for an unlinked shader.");

      return;
    }

    Use();

    const int location = glGetUniformLocation(program_id_, name.c_str());

    if (location == -1)
    {
      utility::logger->warn("Uniform \"{}\" does not exist.", name);

      return;
    }

    glUniformMatrix2fv(location, 1, transpose, glm::value_ptr(data));
  }

  template<>
  void ShaderProgram::SetUniformMatrix(const std::string& name, const glm::mat3& data, uint8_t transpose) const
  {
    if (!is_linked_)
    {
      utility::logger->warn("Cannot set uniform for an unlinked shader.");

      return;
    }

    Use();

    const int location = glGetUniformLocation(program_id_, name.c_str());

    if (location == -1)
    {
      utility::logger->warn("Uniform \"{}\" does not exist.", name);

      return;
    }

    glUniformMatrix3fv(location, 1, transpose, glm::value_ptr(data));
  }

  template<>
  void ShaderProgram::SetUniformMatrix(const std::string& name, const glm::mat4& data, uint8_t transpose) const
  {
    if (!is_linked_)
    {
      utility::logger->warn("Cannot set uniform for an unlinked shader.");

      return;
    }

    Use();

    const int location = glGetUniformLocation(program_id_, name.c_str());

    if (location == -1)
    {
      utility::logger->warn("Uniform \"{}\" does not exist.", name);

      return;
    }

    glUniformMatrix4fv(location, 1, transpose, glm::value_ptr(data));
  }

  template<>
  void ShaderProgram::SetUniformMatrix(const std::string& name, const glm::mat2x3& data, uint8_t transpose) const
  {
    if (!is_linked_)
    {
      utility::logger->warn("Cannot set uniform for an unlinked shader.");

      return;
    }

    Use();

    const int location = glGetUniformLocation(program_id_, name.c_str());

    if (location == -1)
    {
      utility::logger->warn("Uniform \"{}\" does not exist.", name);

      return;
    }

    glUniformMatrix2x3fv(location, 1, transpose, glm::value_ptr(data));
  }

  template<>
  void ShaderProgram::SetUniformMatrix(const std::string& name, const glm::mat2x4& data, uint8_t transpose) const
  {
    if (!is_linked_)
    {
      utility::logger->warn("Cannot set uniform for an unlinked shader.");

      return;
    }

    Use();

    const int location = glGetUniformLocation(program_id_, name.c_str());

    if (location == -1)
    {
      utility::logger->warn("Uniform \"{}\" does not exist.", name);

      return;
    }

    glUniformMatrix2x4fv(location, 1, transpose, glm::value_ptr(data));
  }

  template<>
  void ShaderProgram::SetUniformMatrix(const std::string& name, const glm::mat3x2& data, uint8_t transpose) const
  {
    if (!is_linked_)
    {
      utility::logger->warn("Cannot set uniform for an unlinked shader.");

      return;
    }

    Use();

    const int location = glGetUniformLocation(program_id_, name.c_str());

    if (location == -1)
    {
      utility::logger->warn("Uniform \"{}\" does not exist.", name);

      return;
    }

    glUniformMatrix3x2fv(location, 1, transpose, glm::value_ptr(data));
  }

  template<>
  void ShaderProgram::SetUniformMatrix(const std::string& name, const glm::mat3x4& data, uint8_t transpose) const
  {
    if (!is_linked_)
    {
      utility::logger->warn("Cannot set uniform for an unlinked shader.");

      return;
    }

    Use();

    const int location = glGetUniformLocation(program_id_, name.c_str());

    if (location == -1)
    {
      utility::logger->warn("Uniform \"{}\" does not exist.", name);

      return;
    }

    glUniformMatrix3x4fv(location, 1, transpose, glm::value_ptr(data));
  }

  template<>
  void ShaderProgram::SetUniformMatrix(const std::string& name, const glm::mat4x2& data, uint8_t transpose) const
  {
    if (!is_linked_)
    {
      utility::logger->warn("Cannot set uniform for an unlinked shader.");

      return;
    }

    Use();

    const int location = glGetUniformLocation(program_id_, name.c_str());

    if (location == -1)
    {
      utility::logger->warn("Uniform \"{}\" does not exist.", name);

      return;
    }

    glUniformMatrix4x2fv(location, 1, transpose, glm::value_ptr(data));
  }

  template<>
  void ShaderProgram::SetUniformMatrix(const std::string& name, const glm::mat4x3& data, uint8_t transpose) const
  {
    if (!is_linked_)
    {
      utility::logger->warn("Cannot set uniform for an unlinked shader.");

      return;
    }

    Use();

    const int location = glGetUniformLocation(program_id_, name.c_str());

    if (location == -1)
    {
      utility::logger->warn("Uniform \"{}\" does not exist.", name);

      return;
    }

    glUniformMatrix4x3fv(location, 1, transpose, glm::value_ptr(data));
  }
}