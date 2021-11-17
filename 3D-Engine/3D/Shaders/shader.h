#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <fstream>
#include <string>

#include <GL/gl.h>

class Shader
{

public:

    static Shader* buildVertexShader(std::string shaderPath);
    static Shader* buildFragmentShader(std::string shaderPath);

    const char *shaderSource() const;
    GLenum shaderType() const;

private:

    std::string _shaderSource;
    GLenum _shaderType;

    Shader(std::string shaderPath, GLenum shaderType);

};

#endif // SHADER_H
