#include "openglbase.h"

#include <iostream>

OpenGLBase::OpenGLBase(int width, int height) : _width(width), _height(height), _drawFill(true) {
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, width, height);
}

OpenGLBase::~OpenGLBase() {
}

void OpenGLBase::resize(int width, int height) {
   _width = width;
   _height = height;
    glViewport(0, 0, _width, _height);
}

void OpenGLBase::draw() {
    glClearColor(0.f, 0.f, 0.f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    if (_drawFill)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
}

void OpenGLBase::mouseclick(int , float , float ) {

}

void OpenGLBase::mousemove(float , float ) {

}

void OpenGLBase::mousewheel(float ) {

}

void OpenGLBase::keyboardmove(int , double ) {

}

bool OpenGLBase::keyboard(unsigned char ) {
    return false;
}

void OpenGLBase::loadSceneFromFile(std::string fileName)
{
    std::cerr << "Loading from file unimplemented, " << fileName << " couldn't be loaded" << std::endl;
    exit(1);
}

void OpenGLBase::toggleDrawmode() {
    _drawFill = !_drawFill;
}
