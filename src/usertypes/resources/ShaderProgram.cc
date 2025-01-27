#include <filesystem>
#include "ShaderProgram.h"
#include "../../system/FileFunctions.h"
#include "../../utility/GLUtils.h"
#include "../../utility/ImGuiUtils.h"
#include "../../utility/LogUtils.h"

namespace term_engine::usertypes {
  ShaderProgram::ShaderProgram(const std::string& name, uint32_t program_id, const utility::UniformList& uniforms) :
    BaseResource(name),
    program_id_(program_id),
    uniforms_(uniforms)
  {
    utility::LogDebug("Created shader program resource with ID {} and name \"{}\".", program_id_, name_);
  }

  ShaderProgram::~ShaderProgram()
  {
    utility::LogDebug("Destroyed shader program resource with ID {} and name \"{}\".", program_id_, name_);

    glDeleteProgram(program_id_);
  }

  std::string ShaderProgram::GetResourceType() const
  {
    return std::string(SHADER_PROGRAM_TYPE);
  }

  uint32_t ShaderProgram::GetProgramId() const
  {
    return program_id_;
  }

  void ShaderProgram::Use() const
  {
    glUseProgram(program_id_);
  }

  template<>
  void ShaderProgram::SetUniform(const std::string& name, float data) const
  {
    Use();

    const int location = glGetUniformLocation(program_id_, name.c_str());

    if (location == -1)
    {
      utility::LogWarn("Uniform \"{}\" does not exist.", name);

      return;
    }

    glUniform1f(location, data);
  }

  template<>
  void ShaderProgram::SetUniform(const std::string& name, int data) const
  {
    Use();

    const int location = glGetUniformLocation(program_id_, name.c_str());

    if (location == -1)
    {
      utility::LogWarn("Uniform \"{}\" does not exist.", name);

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
    Use();

    const int location = glGetUniformLocation(program_id_, name.c_str());

    if (location == -1)
    {
      utility::LogWarn("Uniform \"{}\" does not exist.", name);

      return;
    }

    glUniform2fv(location, 1, glm::value_ptr(data));
  }

  template<>
  void ShaderProgram::SetUniformVector(const std::string& name, const glm::ivec2& data) const
  {
    Use();

    const int location = glGetUniformLocation(program_id_, name.c_str());

    if (location == -1)
    {
      utility::LogWarn("Uniform \"{}\" does not exist.", name);

      return;
    }

    glUniform2iv(location, 1, glm::value_ptr(data));
  }

  template<>
  void ShaderProgram::SetUniformVector(const std::string& name, const glm::uvec2& data) const
  {
    Use();

    const int location = glGetUniformLocation(program_id_, name.c_str());

    if (location == -1)
    {
      utility::LogWarn("Uniform \"{}\" does not exist.", name);

      return;
    }

    glUniform2uiv(location, 1, glm::value_ptr(data));
  }

  template<>
  void ShaderProgram::SetUniformVector(const std::string& name, const glm::vec3& data) const
  {
    Use();

    const int location = glGetUniformLocation(program_id_, name.c_str());

    if (location == -1)
    {
      utility::LogWarn("Uniform \"{}\" does not exist.", name);

      return;
    }

    glUniform3fv(location, 1, glm::value_ptr(data));
  }

  template<>
  void ShaderProgram::SetUniformVector(const std::string& name, const glm::ivec3& data) const
  {
    Use();

    const int location = glGetUniformLocation(program_id_, name.c_str());

    if (location == -1)
    {
      utility::LogWarn("Uniform \"{}\" does not exist.", name);

      return;
    }

    glUniform3iv(location, 1, glm::value_ptr(data));
  }

  template<>
  void ShaderProgram::SetUniformVector(const std::string& name, const glm::uvec3& data) const
  {
    Use();

    const int location = glGetUniformLocation(program_id_, name.c_str());

    if (location == -1)
    {
      utility::LogWarn("Uniform \"{}\" does not exist.", name);

      return;
    }

    glUniform3uiv(location, 1, glm::value_ptr(data));
  }

  template<>
  void ShaderProgram::SetUniformVector(const std::string& name, const glm::vec4& data) const
  {
    Use();

    const int location = glGetUniformLocation(program_id_, name.c_str());

    if (location == -1)
    {
      utility::LogWarn("Uniform \"{}\" does not exist.", name);

      return;
    }

    glUniform4fv(location, 1, glm::value_ptr(data));
  }

  template<>
  void ShaderProgram::SetUniformVector(const std::string& name, const glm::ivec4& data) const
  {
    Use();

    const int location = glGetUniformLocation(program_id_, name.c_str());

    if (location == -1)
    {
      utility::LogWarn("Uniform \"{}\" does not exist.", name);

      return;
    }

    glUniform4iv(location, 1, glm::value_ptr(data));
  }

  template<>
  void ShaderProgram::SetUniformVector(const std::string& name, const glm::uvec4& data) const
  {
    Use();

    const int location = glGetUniformLocation(program_id_, name.c_str());

    if (location == -1)
    {
      utility::LogWarn("Uniform \"{}\" does not exist.", name);

      return;
    }

    glUniform4uiv(location, 1, glm::value_ptr(data));
  }

  template<>
  void ShaderProgram::SetUniformMatrix(const std::string& name, const glm::mat2& data, uint8_t transpose) const
  {
    Use();

    const int location = glGetUniformLocation(program_id_, name.c_str());

    if (location == -1)
    {
      utility::LogWarn("Uniform \"{}\" does not exist.", name);

      return;
    }

    glUniformMatrix2fv(location, 1, transpose, glm::value_ptr(data));
  }

  template<>
  void ShaderProgram::SetUniformMatrix(const std::string& name, const glm::mat3& data, uint8_t transpose) const
  {
    Use();

    const int location = glGetUniformLocation(program_id_, name.c_str());

    if (location == -1)
    {
      utility::LogWarn("Uniform \"{}\" does not exist.", name);

      return;
    }

    glUniformMatrix3fv(location, 1, transpose, glm::value_ptr(data));
  }

  template<>
  void ShaderProgram::SetUniformMatrix(const std::string& name, const glm::mat4& data, uint8_t transpose) const
  {
    Use();

    const int location = glGetUniformLocation(program_id_, name.c_str());

    if (location == -1)
    {
      utility::LogWarn("Uniform \"{}\" does not exist.", name);

      return;
    }

    glUniformMatrix4fv(location, 1, transpose, glm::value_ptr(data));
  }

  template<>
  void ShaderProgram::SetUniformMatrix(const std::string& name, const glm::mat2x3& data, uint8_t transpose) const
  {
    Use();

    const int location = glGetUniformLocation(program_id_, name.c_str());

    if (location == -1)
    {
      utility::LogWarn("Uniform \"{}\" does not exist.", name);

      return;
    }

    glUniformMatrix2x3fv(location, 1, transpose, glm::value_ptr(data));
  }

  template<>
  void ShaderProgram::SetUniformMatrix(const std::string& name, const glm::mat2x4& data, uint8_t transpose) const
  {
    Use();

    const int location = glGetUniformLocation(program_id_, name.c_str());

    if (location == -1)
    {
      utility::LogWarn("Uniform \"{}\" does not exist.", name);

      return;
    }

    glUniformMatrix2x4fv(location, 1, transpose, glm::value_ptr(data));
  }

  template<>
  void ShaderProgram::SetUniformMatrix(const std::string& name, const glm::mat3x2& data, uint8_t transpose) const
  {
    Use();

    const int location = glGetUniformLocation(program_id_, name.c_str());

    if (location == -1)
    {
      utility::LogWarn("Uniform \"{}\" does not exist.", name);

      return;
    }

    glUniformMatrix3x2fv(location, 1, transpose, glm::value_ptr(data));
  }

  template<>
  void ShaderProgram::SetUniformMatrix(const std::string& name, const glm::mat3x4& data, uint8_t transpose) const
  {
    Use();

    const int location = glGetUniformLocation(program_id_, name.c_str());

    if (location == -1)
    {
      utility::LogWarn("Uniform \"{}\" does not exist.", name);

      return;
    }

    glUniformMatrix3x4fv(location, 1, transpose, glm::value_ptr(data));
  }

  template<>
  void ShaderProgram::SetUniformMatrix(const std::string& name, const glm::mat4x2& data, uint8_t transpose) const
  {
    Use();

    const int location = glGetUniformLocation(program_id_, name.c_str());

    if (location == -1)
    {
      utility::LogWarn("Uniform \"{}\" does not exist.", name);

      return;
    }

    glUniformMatrix4x2fv(location, 1, transpose, glm::value_ptr(data));
  }

  template<>
  void ShaderProgram::SetUniformMatrix(const std::string& name, const glm::mat4x3& data, uint8_t transpose) const
  {
    Use();

    const int location = glGetUniformLocation(program_id_, name.c_str());

    if (location == -1)
    {
      utility::LogWarn("Uniform \"{}\" does not exist.", name);

      return;
    }

    glUniformMatrix4x3fv(location, 1, transpose, glm::value_ptr(data));
  }

  void ShaderProgram::UpdateDebugInfo() const
  {
    if (ImGui::TreeNode((void*)this, "%s (%s)", GetResourceType().c_str(), name_.c_str()))
    {
      ImGui::Text("Name: %s", name_.c_str());
      ImGui::Text("Program ID: %i", program_id_);

      ImGui::SeparatorText("Uniforms");

      ImGui::Text("Count: %li", uniforms_.size());

      for (const utility::Uniform& uniform : uniforms_)
      {
        if (ImGui::TreeNode((void*)&uniform, "#%i", uniform.location_))
        {
          ImGui::Text("Name: %s", uniform.name_.c_str());
          ImGui::Text("Type: %s (%i)", utility::GetUniformTypeName(uniform.type_).c_str(), uniform.type_);
          ImGui::Text("Count: %i", uniform.count_);

          ImGui::TreePop();
        }
      }

      ImGui::TreePop();
    }
  }

  ShaderProgram* AddShader(const std::string& name, const std::string& vert_code, const std::string& frag_code, const std::string& geom_code)
  {
    if (name.empty())
    {
      utility::LogWarn("Cannot create shader program with empty name!");
      
      return nullptr;
    }

    ResourceList::iterator it = resource_list.find(name);

    if (it != resource_list.end())
    {
      if (it->second->GetResourceType() == std::string(SHADER_PROGRAM_TYPE))
      {
        utility::LogWarn("\"{}\" already exists.", name);
      }
      else
      {
        utility::LogWarn("\"{}\" is the name of a(n) {} resource.", name, it->second->GetResourceType());
      }

      return nullptr;
    }
    
    uint32_t program_id = glCreateProgram();
    utility::ShaderProcessResult vert_results, frag_results, geom_results;

    if (program_id == 0)
    {
      utility::LogError("Failed to create shader program! Error #{}", glGetError());

      return nullptr;
    }

    if (!vert_code.empty())
    {
      vert_results = utility::CompileShaderStage(vert_code, GL_VERTEX_SHADER);

      if (vert_results.second == GL_TRUE)
      {
        glAttachShader(program_id, vert_results.first);
      }
    }

    if (!frag_code.empty())
    {
      frag_results = utility::CompileShaderStage(frag_code, GL_FRAGMENT_SHADER);

      if (frag_results.second == GL_TRUE)
      {
        glAttachShader(program_id, frag_results.first);
      }
    }

    if (!geom_code.empty())
    {
      geom_results = utility::CompileShaderStage(geom_code, GL_GEOMETRY_SHADER);

      if (geom_results.second == GL_TRUE)
      {
        glAttachShader(program_id, geom_results.first);
      }
    }

    uint8_t all_compiled = GL_TRUE & vert_results.second & frag_results.second & (geom_results.first > 0 ? geom_results.second : GL_TRUE);
    utility::ShaderProcessResult link_results = utility::LinkShaderProgram(program_id);

    if (glIsShader(vert_results.first))
    {
      utility::PrintStageLog(vert_results.first);

      glDetachShader(program_id, vert_results.first);
      glDeleteShader(vert_results.first);
    }

    if (glIsShader(frag_results.first))
    {
      utility::PrintStageLog(frag_results.first);
      
      glDetachShader(program_id, frag_results.first);
      glDeleteShader(frag_results.first);
    }

    if (glIsShader(geom_results.first))
    {
      utility::PrintStageLog(geom_results.first);
      
      glDetachShader(program_id, geom_results.first);
      glDeleteShader(geom_results.first);
    }

    if (link_results.second == GL_TRUE && all_compiled)
    {
      utility::PrintProgramLog(program_id);

      utility::UniformList uniforms = utility::GetUniforms(program_id);

      auto new_shader = resource_list.emplace(name, std::make_unique<ShaderProgram>(name, program_id, uniforms));

      if (new_shader.second)
      {
        return static_cast<ShaderProgram*>(new_shader.first->second.get());
      }
    }

    utility::LogError("Failed to compile and link shader \"{}\"!", name);

    glDeleteProgram(program_id);

    return nullptr;
  }

  ShaderProgram* GetShader(const std::string& name)
  {
    ResourceList::iterator it = resource_list.find(name);

    if (it != resource_list.end() && it->second->GetResourceType() != std::string(SHADER_PROGRAM_TYPE))
    {
      utility::LogWarn("\"{}\" is the name of a(n) {} resource.", name, it->second->GetResourceType());

      return nullptr;
    }
    else if (it != resource_list.end())
    {
      return static_cast<ShaderProgram*>(it->second.get());
    }
    else
    {
      utility::LogWarn("Shader with name \"{}\" does not exist.", name);

      return nullptr;
    }
  }
}