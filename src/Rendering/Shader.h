#pragma once
#include <fstream>
#include <sstream>
#include <string>

#include "glad/glad.h"
#include "glm/fwd.hpp"


class Shader
{
public:
    unsigned int ShaderProgramID;

public:
    Shader(const std::string& vertexPath, const std::string& fragmentPath);
    ~Shader();

    void Use();

    void SetFloat(const std::string& name, float value) const;
    void SetBool(const std::string& name, bool value) const;
    void SetInt(const std::string& name, int value) const;

};
