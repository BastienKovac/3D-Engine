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

    loadShader("base", "../3D-Engine/Resources/Shaders/BaseVertexShader.vert", "../3D-Engine/Resources/Shaders/BaseFragmentShader.frag");
    loadShader("skybox", "../3D-Engine/Resources/Shaders/SkyboxVertexShader.vert", "../3D-Engine/Resources/Shaders/SkyboxFragmentShader.frag");

    refreshScene();
}

void SimpleScene::clearOpenGLContext()
{
    glDeleteBuffers(1, &_vbo);
    glDeleteBuffers(1, &_nbo);
    glDeleteBuffers(1, &_ebo);
    glDeleteBuffers(1, &_skyboxVBO);
    glDeleteVertexArrays(1, &_vao) ;
    glDeleteVertexArrays(1, &_skyboxVAO);
}

SimpleScene::~SimpleScene() {
    clearOpenGLContext();
}

void SimpleScene::resize(int width, int height){
    OpenGLBase::resize(width, height);
    _camera->setviewport(glm::vec4(0.f, 0.f, _width, _height));
    _projection = glm::perspective(_camera->zoom(), float(_width) / _height, 0.1f, 100.0f);
}

void SimpleScene::initSkyBoxBuffers()
{
    glGenVertexArrays(1, &_skyboxVAO);
    glGenBuffers(1, &_skyboxVBO);

    glBindVertexArray(_skyboxVAO);

    glBindBuffer(GL_ARRAY_BUFFER, _skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(_skyboxVertices), &_skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);

    useShader("skybox");
    glUniform1d(glGetUniformLocation(_programID, "skybox"), 0);
}

void SimpleScene::renderSkyBox()
{
    glDepthMask(GL_FALSE);
    useShader("skybox");

    glUniformMatrix4fv(glGetUniformLocation(_programID, "model"), 1, GL_FALSE, glm::value_ptr(_model));
    // Remove translation from view matrix
    glUniformMatrix4fv(glGetUniformLocation(_programID, "view"), 1, GL_FALSE, glm::value_ptr(glm::mat4(glm::mat3(_view))));
    glUniformMatrix4fv(glGetUniformLocation(_programID, "projection"), 1, GL_FALSE, glm::value_ptr(_projection));

    glBindVertexArray(_skyboxVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, _skyboxID);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

    glDepthMask(GL_TRUE);
}

void SimpleScene::initGeometryBuffers()
{
    glGenBuffers(1, &_vbo);
    glGenBuffers(1, &_nbo);
    glGenBuffers(1, &_ebo);
    glGenVertexArrays(1, &_vao);

    glBindVertexArray(_vao);

    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, _geometry.get()->vertices().size() * sizeof (GLfloat), _geometry.get()->vertices().data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, _nbo);
    glBufferData(GL_ARRAY_BUFFER, _geometry.get()->normals().size() * sizeof (GLfloat), _geometry.get()->normals().data(), GL_STATIC_DRAW);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _geometry.get()->indices().size() * sizeof (GLfloat), _geometry.get()->indices().data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void SimpleScene::renderGeometry()
{
    useShader("base");

    glUniformMatrix4fv(glGetUniformLocation(_programID, "model"), 1, GL_FALSE, glm::value_ptr(_model));
    glUniformMatrix4fv(glGetUniformLocation(_programID, "view"), 1, GL_FALSE, glm::value_ptr(_view));
    glUniformMatrix4fv(glGetUniformLocation(_programID, "projection"), 1, GL_FALSE, glm::value_ptr(_projection));

    glBindVertexArray(_vao);
    glDrawElements(GL_TRIANGLES, _geometry.get()->indices().size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void SimpleScene::draw() {
    OpenGLBase::draw();
    _view = _camera->viewmatrix();

    if (_hasSkybox)
    {
        renderSkyBox();
    }

    renderGeometry();

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

    initGeometryBuffers();
    if (_hasSkybox)
    {
        initSkyBoxBuffers();
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

void SimpleScene::loadShader(std::string name, std::string vertexPath, std::string fragmentPath)
{
    _shaderMaps.erase(name);

    Shader* shader = new Shader(vertexPath, fragmentPath);

    _shaderMaps.insert(std::make_pair(name, shader));
    refreshScene();
}

void SimpleScene::useShader(std::string name)
{
    if (name == _currentShader)
    {
        // Correct shader is already loaded, return
        return;
    }

    _currentShader = name;

    Shader* shader = _shaderMaps[name];

    auto path = std::filesystem::path(shader->vertexPath());
    std::ifstream v_in(path);
    std::string v_contents((std::istreambuf_iterator<char>(v_in)), std::istreambuf_iterator<char>());

    GLint success;
    GLchar infoLog[512]; // warning fixed size ... request for LOG_LENGTH!!!

    GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
    const char* v_source = v_contents.c_str();
    glShaderSource(vertex, 1, &v_source, NULL);
    glCompileShader(vertex);

    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    path = std::filesystem::path(shader->fragmentPath());
    std::ifstream f_in(path);
    std::string f_contents((std::istreambuf_iterator<char>(f_in)), std::istreambuf_iterator<char>());

    GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
    const char* f_source = f_contents.c_str();
    glShaderSource(fragment, 1, &f_source, NULL);
    glCompileShader(fragment);

    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // shader Program
    _programID = glCreateProgram();
    glAttachShader(_programID, vertex);
    glAttachShader(_programID, fragment);
    glLinkProgram(_programID);

    glGetProgramiv(_programID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(_programID, 512, NULL, infoLog);
        std::cerr << "ERROR::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    glUseProgram(_programID);
}

void SimpleScene::loadSkybox(std::string path)
{
    std::vector<std::string> expectedFileNamesInOrder = {"right", "left", "top", "bottom", "front", "back"};
    std::map<std::string, std::filesystem::path> files;

    for (const auto & entry : std::filesystem::directory_iterator(path))
    {
        if (std::find(expectedFileNamesInOrder.begin(), expectedFileNamesInOrder.end(), entry.path().stem()) != expectedFileNamesInOrder.end())
        {
            files[entry.path().stem()] = entry.path();
        }
    }

    if (expectedFileNamesInOrder.size() == files.size())
    {
        int i = 0;
        unsigned int textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

        int width, height, nrChannels;
        for (std::string key : expectedFileNamesInOrder)
        {
            unsigned char *data = stbi_load(files[key].c_str(), &width, &height, &nrChannels, 0);
            if (data)
            {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                stbi_image_free(data);

                ++i;
            }
            else
            {
                std::cerr << "Cubemap tex failed to load at path: " << path << std::endl;
                stbi_image_free(data);

                return;
            }
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        _hasSkybox = true;
        _skyboxID = textureID;

        refreshScene();
    }
    else
    {
        std::cerr << "Missing skybox files" << std::endl;
    }
}

unsigned int SimpleScene::loadTexture(char const* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
        {

            format = GL_RED;
        }
        else if (nrComponents == 3)
        {

            format = GL_RGB;
        }
        else if (nrComponents == 4)
        {
            format = GL_RGBA;
        }

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}
