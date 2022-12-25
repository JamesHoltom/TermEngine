#include <filesystem>
#include "ShaderProgram.h"
#include "../system/FileFunctions.h"
#include "../utility/SpdlogUtils.h"

namespace shaders {
  ShaderProgram::ShaderProgram() :
    linked_(false),
    shader_vert_id_(0),
    shader_frag_id_(0),
    shader_geom_id_(0)
  {
    program_id_ = glCreateProgram();
  }

  ShaderProgram::~ShaderProgram()
  {
    RemoveCompiledShaders();

    glDeleteProgram(program_id_);
  }

  GLuint ShaderProgram::GetProgramId() const
  {
    return program_id_;
  }

  void ShaderProgram::AttachFile(const std::string& filepath, const GLenum& type)
  {
    const std::filesystem::path fullpath = system::SearchForResourcePath(filepath);
    const std::string glsl_code = system::ReadFile(fullpath);

    AttachString(glsl_code, type);
  }

  void ShaderProgram::AttachString(const std::string& glsl_code, const GLenum& type)
  {
    if (glsl_code != "")
    {
      GL::ShaderProcessResult results = GL::CompileShaderStage(glsl_code, type);
      GL::PrintStageLog(results.id_);

      if (results.success_ == GL_TRUE)
      {
        switch (type)
        {
          case GL_VERTEX_SHADER:    shader_vert_id_ = results.id_;  break;
          case GL_FRAGMENT_SHADER:  shader_frag_id_ = results.id_;  break;
          case GL_GEOMETRY_SHADER:  shader_geom_id_ = results.id_;  break;
        }

        glAttachShader(program_id_, results.id_);

        logging::logger->debug("Compiled {} shader with ID {}.", GL::GetShaderTypeName(type), results.id_);
      }
      else
      {
        logging::logger->error("Failed to compile {} shader with ID {}.", GL::GetShaderTypeName(type), results.id_);
      }
    }
    else
    {
      logging::logger->warn("No GLSL code to compile!");
    }
  }

  void ShaderProgram::Link()
  {
    GL::ShaderProcessResult results = GL::LinkShaderProgram(program_id_);
    GL::PrintProgramLog(program_id_);

    if (results.success_ == GL_TRUE)
    {
      RemoveCompiledShaders();

      logging::logger->debug("Linked shader program with ID {}.", program_id_);
    }
    else
    {
      logging::logger->debug("Failed to link shader program with ID {}.", program_id_);
    }
  }

  void ShaderProgram::Use() const
  {
    program_in_use_ = program_id_;
  }

  GLuint ShaderProgram::GetProgramInUse()
  {
    return program_in_use_;
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

  GLuint ShaderProgram::program_in_use_ = 0;
  ShaderProgramList ShaderProgram::shader_list_;
}