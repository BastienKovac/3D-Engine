#ifndef OPENGLGEOMETRY_H
#define OPENGLGEOMETRY_H

#include <3D/openglbase.h>
#include <3D/Camera/camera.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <memory>
#include <functional>

class OpenGLGeometry : public OpenGLBase
{
public:
    explicit OpenGLGeometry(int width, int height);
    ~OpenGLGeometry() override;

    void resize(int width, int height) override;
    void draw() override;

    void mouseclick(int button, float xpos, float ypos) override;
    void mousemove(float xpos, float ypos) override;
    void keyboardmove(int key, double time) override;
    bool keyboard(unsigned char k) override;

    void setGeometry(std::vector<GLfloat> vertices, std::vector<GLuint> indices);

private:
    // A simple geometry
    std::vector<GLfloat> _vertices;
    std::vector<GLfloat> _normals;
    std::vector<GLuint> _indices;

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

    // Compute normals
    void computeNormalsFor(std::vector<GLfloat> vertices, std::vector<GLuint> indices);
    glm::vec3 computeNormalFor(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);

    void clearOpenGLContext();
};

#endif // OPENGLGEOMETRY_H