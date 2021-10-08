#ifndef OPENGLGEOMETRY_H
#define OPENGLGEOMETRY_H

#include <3D/openglbase.h>
#include <3D/Camera/camera.h>
#include <3D/Geometry/geometry.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <memory>
#include <functional>
#include <string>

class SimpleScene : public OpenGLBase
{
public:
    explicit SimpleScene(int width, int height);
    ~SimpleScene() override;

    void resize(int width, int height) override;
    void draw() override;

    void mouseclick(int button, float xpos, float ypos) override;
    void mousemove(float xpos, float ypos) override;
    void keyboardmove(int key, double time) override;
    bool keyboard(unsigned char k) override;

    void refreshScene();

    void loadSceneFromFile(std::string fileName) override;

private:
    // A simple geometry
    std::unique_ptr<Geometry> _geometry;

    // OpenGL object for geometry
    GLuint _vao;
    GLuint _vbo;
    GLuint _nbo;
    GLuint _ebo;

    // Shader program for rendering
    GLuint _program;

    // for mouse management
    int _button; // 0 --> left. 1 --> right. 2 --> middle. 3 --> other
    float _mousex{0};
    float _mousey{0};

    // Camera
    using CameraSelector = std::function<Camera*()>;
    std::vector<CameraSelector> _cameraselector;
    unsigned int _activecamera;

    std::unique_ptr<Camera> _camera;

    // matrices
    glm::mat4 _model;
    glm::mat4 _view;
    glm::mat4 _projection;

    void clearOpenGLContext();
};

#endif // OPENGLGEOMETRY_H
