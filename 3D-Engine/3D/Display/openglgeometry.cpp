#include "openglgeometry.h"
#include <iostream>

#define deg2rad(x) float(M_PI)*(x)/180.f

static const char* vertexshader_source ="#version 410 core\n\
        layout (location = 0) in vec3 position;\n\
        layout (location = 1) in vec3 inormal;\n\
        uniform mat4 model;\n\
        uniform mat4 view;\n\
        uniform mat4 projection;\n\
        out vec3 normal;\n\
        void main()\n\
        {\n\
            // Note that we read the multiplication from right to left\n\
            gl_Position = projection * view * model * vec4(position, 1.0f);\n\
            normal = inormal;\n\
        }\n";

static const char* fragmentshader_source ="#version 410 core\n\
        in vec3 normal;\n\
        out vec4 color;\n\
        void main()\n\
        {\n\
            //color = vec4(vec3(clamp(dot(normalize(normal), vec3(0,0,1)), 0, 1)), 1.0);\n\
            color = vec4(normalize(normal)*0.5+0.5, 1.0);\n\
        }\n";

OpenGLGeometry::OpenGLGeometry(int width, int height) : OpenGLBase(width, height), _activecamera(0), _camera(nullptr) {
    // Initialise geometric data
    std::vector<GLfloat> vertices = {
        0.5f,  0.5f, 0.0f,  // Top Right
        0.5f, -0.5f, 0.0f,  // Bottom Right
       -0.5f, -0.5f, 0.0f,  // Bottom Left
       -0.5f,  0.5f, 0.0f   // Top Left
    };

    std::vector<GLuint> indices = {
        0, 1, 3,   // First Triangle
        1, 2, 3    // Second Triangle
    };

    setGeometry(vertices, indices);
}

void OpenGLGeometry::clearOpenGLContext()
{
    glDeleteProgram(_program);
    glDeleteBuffers(1, &_vbo);
    glDeleteBuffers(1, &_nbo);
    glDeleteBuffers(1, &_ebo);
    glDeleteVertexArrays(1, &_vao) ;
}

OpenGLGeometry::~OpenGLGeometry() {
    clearOpenGLContext();
}

void OpenGLGeometry::resize(int width, int height){
    OpenGLBase::resize(width, height);
    _camera->setviewport(glm::vec4(0.f, 0.f, _width, _height));
    _projection = glm::perspective(_camera->zoom(), float(_width) / _height, 0.1f, 100.0f);
}

void OpenGLGeometry::draw() {
    OpenGLBase::draw();

    glUseProgram(_program);

    _view = _camera->viewmatrix();

    glUniformMatrix4fv(glGetUniformLocation(_program, "model"), 1, GL_FALSE, glm::value_ptr(_model));
    glUniformMatrix4fv(glGetUniformLocation(_program, "view"), 1, GL_FALSE, glm::value_ptr(_view));
    glUniformMatrix4fv(glGetUniformLocation(_program, "projection"), 1, GL_FALSE, glm::value_ptr(_projection));

    glBindVertexArray(_vao);
    glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, NULL);
    glBindVertexArray(0);
}

void OpenGLGeometry::mouseclick(int button, float xpos, float ypos) {
    _button = button;
    _mousex = xpos;
    _mousey = ypos;
    _camera->processmouseclick(_button, xpos, ypos);
}

void OpenGLGeometry::mousemove(float xpos, float ypos) {
    _camera->processmousemovement(_button, xpos, ypos, true);
}

void OpenGLGeometry::keyboardmove(int key, double time) {
    _camera->processkeyboard(Camera_Movement(key), time);
}

bool OpenGLGeometry::keyboard(unsigned char k) {
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

void OpenGLGeometry::setGeometry(std::vector<GLfloat> vertices, std::vector<GLuint> indices)
{
    clearOpenGLContext();

    _vertices.clear();
    _vertices.insert(_vertices.end(), vertices.begin(), vertices.end());

    _indices.clear();
    _indices.insert(_indices.end(), indices.begin(), indices.end());

    computeNormalsFor(vertices, indices);

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
    glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof (GLfloat), _vertices.data(), GL_STATIC_DRAW);

    // 4. Then set our vertex attributes pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // 5. Copy our normals array in a buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, _nbo);
    glBufferData(GL_ARRAY_BUFFER, _normals.size() * sizeof (GLfloat), _normals.data(), GL_STATIC_DRAW);

    // 6. Copy our vertices array in a buffer for OpenGL to use
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(1);

    // 7. Copy our index array in a element buffer for OpenGL to use
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof (GLfloat), _indices.data(), GL_STATIC_DRAW);

    //6. Unbind the VAO
    glBindVertexArray(0);

    // Initialize shaders
    GLint success;
    GLchar infoLog[512]; // warning fixed size ... request for LOG_LENGTH!!!
    GLuint vertexshader, fragmentshader;

    // 1. Generate the shader
    vertexshader = glCreateShader(GL_VERTEX_SHADER);
    // 2. set the source
    glShaderSource(vertexshader, 1, &vertexshader_source, NULL);
    // 3. Compile
    glCompileShader(vertexshader);
    // 4. test for compile error
    glGetShaderiv(vertexshader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexshader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    fragmentshader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentshader, 1, &fragmentshader_source, NULL);
    glCompileShader(fragmentshader);
    glGetShaderiv(fragmentshader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentshader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // 1. Generate the program
    _program = glCreateProgram();
    // 2. Attach the shaders to the program
    glAttachShader(_program, vertexshader);
    glAttachShader(_program, fragmentshader);
    // 3. Link the program
    glLinkProgram(_program);
    // 4. Test for link errors
    glGetProgramiv(_program, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(_program, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::LINK_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexshader);
    glDeleteShader(fragmentshader);

    _cameraselector.push_back([]()->Camera*{return new EulerCamera(glm::vec3(0.f, 0.f, 1.f));} );
    _cameraselector.push_back([]()->Camera*{return new TrackballCamera(glm::vec3(0.f, 0.f, 1.f),glm::vec3(0.f, 1.f, 0.f),glm::vec3(0.f, 0.f, 0.f));} );

    _camera.reset(_cameraselector[_activecamera]());

    _camera->setviewport(glm::vec4(0.f, 0.f, _width, _height));
    _view = _camera->viewmatrix();

    _projection = glm::perspective(_camera->zoom(), float(_width) / _height, 0.1f, 100.0f);
}

void OpenGLGeometry::computeNormalsFor(std::vector<GLfloat> vertices, std::vector<GLuint> indices)
{
    std::vector<glm::vec3> normals = std::vector<glm::vec3>(vertices.size() / 3);

    for (unsigned int i = 0 ; i < indices.size() ; i += 3)
    {
        glm::vec3 a = glm::vec3(vertices[indices[i] * 3], vertices[indices[i] * 3 + 1], vertices[indices[i] * 3 + 2]);
        glm::vec3 b = glm::vec3(vertices[indices[i + 1] * 3], vertices[indices[i + 1] * 3 + 1], vertices[indices[i + 1] * 3 + 2]);
        glm::vec3 c = glm::vec3(vertices[indices[i + 2] * 3], vertices[indices[i + 2] * 3 + 1], vertices[indices[i + 2] * 3 + 2]);

        glm::vec3 normal = computeNormalFor(a, b, c);

        normals[indices[i]] += normal;
        normals[indices[i + 1]] += normal;
        normals[indices[i + 2]] += normal;
    }

    for (unsigned int i = 0 ; i < normals.size() ; ++i)
    {
        normals[i] = glm::normalize(normals[i]);

        _normals.push_back(normals[i][0]);
        _normals.push_back(normals[i][1]);
        _normals.push_back(normals[i][2]);
    }
}

glm::vec3 OpenGLGeometry::computeNormalFor(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3)
{
    // Uses p2 as a new origin for p1,p3
    glm::vec3 ab = p2 - p1;
    glm::vec3 ac = p3 - p1;

    // Compute the cross product a X b to get the face normal
    return glm::normalize(glm::cross(ab, ac));
}
