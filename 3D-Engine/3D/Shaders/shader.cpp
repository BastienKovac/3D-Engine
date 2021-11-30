#include "shader.h"

const std::string &Shader::vertexPath() const
{
    return _vertexPath;
}

const std::string &Shader::fragmentPath() const
{
    return _fragmentPath;
}

Shader::Shader(std::string vertexShaderPath, std::string fragmentShaderPath)
{
    _vertexPath = vertexShaderPath;
    _fragmentPath = fragmentShaderPath;
}
