#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

#include <QOpenGLFunctions_4_1_Core>

#include <GL/gl.h>

class Shader
{

public:

    Shader(std::string vertexShaderPath, std::string fragmentShaderPath);

    const std::string &vertexPath() const;
    const std::string &fragmentPath() const;

private:

    std::string _vertexPath;
    std::string _fragmentPath;

};

#endif // SHADER_H
