#ifndef OPENGLGEOMETRY_H
#define OPENGLGEOMETRY_H

#include <3D/Camera/camera.h>
#include <3D/Geometry/geometry.h>
#include <3D/Shaders/shader.h>
#include <3D/stb_image.h>

#include <QOpenGLFunctions_4_1_Core>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <memory>
#include <functional>
#include <string>
#include <map>
#include <iostream>
#include <filesystem>
#include <algorithm>

class SimpleScene : public QOpenGLFunctions_4_1_Core
{
public:
    explicit SimpleScene(int width, int height);
    ~SimpleScene() override;

    long getNumberOfTriangles();

    void resize(int width, int height);
    void draw();

    void mouseclick(int button, float xpos, float ypos);
    void mousemove(float xpos, float ypos);
    void keyboardmove(int key, double time);
    bool keyboard(unsigned char k);

    void toggleDrawmode();

    void refreshScene();

    void subdivideScene();

    void loadSceneFromFile(std::string fileName);
    void loadSkybox(std::string path);

    
private:

    void renderSkyBox();
    void renderGeometry();
    void initSkyBoxBuffers();
    void initGeometryBuffers();

    int _width, _height;
    bool _drawFill;

    // A simple list of object geometries
    std::unique_ptr<Geometry> _geometry;

    // Loaded shaders
    void loadShader(std::string name, std::string vertexPath, std::string fragmentPath);
    void useShader(std::string name);
    std::map<std::string, Shader*> _shaderMaps;

    std::string _currentShader;

    unsigned int _programID;

    // OpenGL object for geometry
    GLuint _vao;
    GLuint _vbo;
    GLuint _nbo;
    GLuint _ebo;

    // for mouse management
    int _button; // 0 --> left. 1 --> right. 2 --> middle. 3 --> other
    float _mousex{0};
    float _mousey{0};

    // Camera
    using CameraSelector = std::function<Camera*()>;
    std::vector<CameraSelector> _cameraselector;
    unsigned int _activecamera = 0;

    std::unique_ptr<Camera> _camera;

    // matrices
    glm::mat4 _model;
    glm::mat4 _view;
    glm::mat4 _projection;

    // Skybox
    bool _hasSkybox = false;
    unsigned int _skyboxID;
    GLuint _skyboxVAO, _skyboxVBO;

    // Skybox positions are absolute
    float _skyboxVertices[36 * 3] = {
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };
;

    void clearOpenGLContext();

    unsigned int loadTexture(const char* path);
};

#endif // OPENGLGEOMETRY_H
