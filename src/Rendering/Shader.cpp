#include "Shader.h"

#include <iostream>

#include "glm/gtc/type_ptr.inl"

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
{
    std::ifstream vertexFile(vertexPath);
    std::ifstream fragmentFile(fragmentPath);
    std::stringstream vertexStreamBuffer;
    std::stringstream fragmentStreamBuffer;

    vertexStreamBuffer << vertexFile.rdbuf();
    fragmentStreamBuffer << fragmentFile.rdbuf();

    std::string vertexCode = vertexStreamBuffer.str();
    std::string fragmentCode = fragmentStreamBuffer.str();
    const char* vertexSource = vertexCode.c_str();
    const char* fragmentSource = fragmentCode.c_str();

    vertexFile.close();
    fragmentFile.close();
    vertexStreamBuffer.clear();
    fragmentStreamBuffer.clear();

    unsigned int vertexShaderId, fragmentShaderId;

    vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShaderId, 1, &vertexSource, NULL);
    glCompileShader(vertexShaderId);
    fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderId, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShaderId);

    ShaderProgramID = glCreateProgram();
    glAttachShader(ShaderProgramID, vertexShaderId);
    glAttachShader(ShaderProgramID, fragmentShaderId);

    glLinkProgram(ShaderProgramID);

    glDeleteShader(vertexShaderId);
    glDeleteShader(fragmentShaderId);
}

Shader::~Shader()
{
    glDeleteProgram(ShaderProgramID);
}

void Shader::Use()
{
    glUseProgram(ShaderProgramID);
}

void Shader::SetFloat(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(ShaderProgramID, name.c_str()), value);
}

void Shader::SetBool(const std::string& name, bool value) const
{
    glUniform1i(glGetUniformLocation(ShaderProgramID, name.c_str()), (int)value);
}

void Shader::SetInt(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(ShaderProgramID, name.c_str()), value);
}


