#include "shader.h"


Shader* Shader::buildVertexShader(std::string shaderPath)
{
    return new Shader(shaderPath, GL_VERTEX_SHADER);
}

Shader* Shader::buildFragmentShader(std::string shaderPath)
{
    return new Shader(shaderPath, GL_FRAGMENT_SHADER);
}

const char *Shader::shaderSource() const
{
    return _shaderSource.c_str();
}

GLenum Shader::shaderType() const
{
    return _shaderType;
}

Shader::Shader(std::string shaderPath, GLenum shaderType)
{
    std::ifstream in(shaderPath);
    std::string contents((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());

    _shaderSource = contents;
    _shaderType = shaderType;
}
