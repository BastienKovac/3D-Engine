#include "simplescene.h"

#define deg2rad(x) float(M_PI)*(x)/180.f

SimpleScene::SimpleScene(int width, int height) : OpenGLBase(width, height), _activecamera(0), _camera(nullptr) {
    _geometry.reset(new Geometry());

    _cameraselector.push_back([]()->Camera*{return new EulerCamera(glm::vec3(0.f, 0.f, 1.f));} );
    _cameraselector.push_back([]()->Camera*{return new TrackballCamera(glm::vec3(0.f, 0.f, 1.f),glm::vec3(0.f, 1.f, 0.f),glm::vec3(0.f, 0.f, 0.f));} );

    _camera.reset(_cameraselector[_activecamera]());

    _camera->setviewport(glm::vec4(0.f, 0.f, _width, _height));
    _view = _camera->viewmatrix();

    _projection = glm::perspective(_camera->zoom(), float(_width) / _height, 0.1f, 100.0f);

    _shaderMaps.insert(std::make_pair(GL_VERTEX_SHADER, Shader::buildVertexShader("3D/Shaders/Shaders/BaseVertexShader.vert")));
    _shaderMaps.insert(std::make_pair(GL_FRAGMENT_SHADER, Shader::buildFragmentShader("3D/Shaders/Shaders/BaseFragmentShader.frag")));

    refreshScene();
}

void SimpleScene::clearOpenGLContext()
{
    glDeleteProgram(_program);
    glDeleteBuffers(1, &_vbo);
    glDeleteBuffers(1, &_nbo);
    glDeleteBuffers(1, &_ebo);
    glDeleteVertexArrays(1, &_vao) ;
}

SimpleScene::~SimpleScene() {
    clearOpenGLContext();
}

void SimpleScene::resize(int width, int height){
    OpenGLBase::resize(width, height);
    _camera->setviewport(glm::vec4(0.f, 0.f, _width, _height));
    _projection = glm::perspective(_camera->zoom(), float(_width) / _height, 0.1f, 100.0f);
}

void SimpleScene::draw() {
    OpenGLBase::draw();

    glUseProgram(_program);

    _view = _camera->viewmatrix();

    glUniformMatrix4fv(glGetUniformLocation(_program, "model"), 1, GL_FALSE, glm::value_ptr(_model));
    glUniformMatrix4fv(glGetUniformLocation(_program, "view"), 1, GL_FALSE, glm::value_ptr(_view));
    glUniformMatrix4fv(glGetUniformLocation(_program, "projection"), 1, GL_FALSE, glm::value_ptr(_projection));

    glBindVertexArray(_vao);
    glDrawElements(GL_TRIANGLES, _geometry.get()->indices().size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void SimpleScene::mouseclick(int button, float xpos, float ypos) {
    _button = button;
    _mousex = xpos;
    _mousey = ypos;
    _camera->processmouseclick(_button, xpos, ypos);
}

void SimpleScene::mousemove(float xpos, float ypos) {
    _camera->processmousemovement(_button, xpos, ypos, true);
}

void SimpleScene::keyboardmove(int key, double time) {
    _camera->processkeyboard(Camera_Movement(key), time);
}

bool SimpleScene::keyboard(unsigned char k) {
    switch(k)
    {
    case 'p':
        _activecamera = (_activecamera + 1) % 2;
        _camera.reset(_cameraselector[_activecamera]());
        _camera->setviewport(glm::vec4(0.f, 0.f, _width, _height));
        return true;
    default:
        return false;
    }
}

void SimpleScene::refreshScene()
{
    clearOpenGLContext();

    // Initialize the geometry
    // 1. Generate geometry buffers
    glGenBuffers(1, &_vbo) ;
    glGenBuffers(1, &_nbo) ;
    glGenBuffers(1, &_ebo) ;
    glGenVertexArrays(1, &_vao) ;

    // 2. Bind Vertex Array Object
    glBindVertexArray(_vao);

    // 3. Copy our vertices array in a buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, _geometry.get()->vertices().size() * sizeof (GLfloat), _geometry.get()->vertices().data(), GL_STATIC_DRAW);

    // 4. Then set our vertex attributes pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // 5. Copy our normals array in a buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, _nbo);
    glBufferData(GL_ARRAY_BUFFER, _geometry.get()->normals().size() * sizeof (GLfloat), _geometry.get()->normals().data(), GL_STATIC_DRAW);

    // 6. Copy our vertices array in a buffer for OpenGL to use
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(1);

    // 7. Copy our index array in a element buffer for OpenGL to use
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _geometry.get()->indices().size() * sizeof (GLfloat), _geometry.get()->indices().data(), GL_STATIC_DRAW);

    //6. Unbind the VAO
    glBindVertexArray(0);

    // Initialize shaders
    GLint success;
    GLchar infoLog[512]; // warning fixed size ... request for LOG_LENGTH!!!

    std::vector<GLuint> shaders;

    for(auto it = _shaderMaps.begin(); it != _shaderMaps.end(); ++it)
    {
        Shader* loadedShader = it->second;
        GLuint shader = glCreateShader(loadedShader->shaderType());

        const char* source = loadedShader->shaderSource();
        glShaderSource(shader, 1, &source, NULL);
        glCompileShader(shader);

        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 512, NULL, infoLog);
            std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        }

        shaders.push_back(shader);
    }

    // 1. Generate the program
    _program = glCreateProgram();

    // 2. Attach the shaders to the program
    for (GLuint shader : shaders)
    {
        glAttachShader(_program, shader);
    }

    // 3. Link the program
    glLinkProgram(_program);
    // 4. Test for link errors
    glGetProgramiv(_program, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(_program, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::LINK_FAILED\n" << infoLog << std::endl;
    }

    for (GLuint shader : shaders)
    {
         glDeleteShader(shader);
    }
}

void SimpleScene::subdivideScene()
{
    _geometry->subdivide();
    refreshScene();
}

void SimpleScene::loadSceneFromFile(std::string fileName)
{
    _geometry->setScene(fileName);
    refreshScene();
}

void SimpleScene::loadShader(GLuint type, std::string path)
{
    switch (type)
    {
    case GL_VERTEX_SHADER:
        _shaderMaps.erase(type);
        _shaderMaps.insert(std::make_pair(type, Shader::buildVertexShader(path)));
        refreshScene();
        break;
    case GL_FRAGMENT_SHADER:
        _shaderMaps.erase(type);
        _shaderMaps.insert(std::make_pair(type, Shader::buildFragmentShader(path)));
        refreshScene();
        break;
    default:
        break;
    }
}
