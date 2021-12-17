#include "simplescene.h"

#define deg2rad(x) float(M_PI)*(x)/180.f

SimpleScene::SimpleScene(int width, int height)
{
    _width = width;
    _height = height;

    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, width, height);

    _geometry.reset(new Geometry());

    _cameraselector.push_back([]()->Camera*{return new EulerCamera(glm::vec3(0.f, 0.f, 1.f));} );
    _cameraselector.push_back([]()->Camera*{return new TrackballCamera(glm::vec3(0.f, 0.f, 1.f),glm::vec3(0.f, 1.f, 0.f),glm::vec3(0.f, 0.f, 0.f));} );

    _camera.reset(_cameraselector[_activecamera]());

    _camera->setviewport(glm::vec4(0.f, 0.f, _width, _height));
    _view = _camera->viewmatrix();

    _projection = glm::perspective(_camera->zoom(), float(_width) / _height, 0.1f, 100.0f);

    loadShader(BASE_SHADER, "../3D-Engine/Resources/Shaders/BaseVertexShader.vert", "../3D-Engine/Resources/Shaders/BaseFragmentShader.frag");
    loadShader(SKYBOX_SHADER, "../3D-Engine/Resources/Shaders/SkyboxVertexShader.vert", "../3D-Engine/Resources/Shaders/SkyboxFragmentShader.frag");
    loadShader(BASE_SHADOW_SHADER, "../3D-Engine/Resources/Shaders/BaseShadowShader.vert", "../3D-Engine/Resources/Shaders/BaseShadowShader.frag");
    loadShader(DEPTH_SHADOW_SHADER, "../3D-Engine/Resources/Shaders/DepthShadow.vert", "../3D-Engine/Resources/Shaders/DepthShadow.frag");
    loadShader(ANIMATION_SHADER, "../3D-Engine/Resources/Shaders/Animation.vert", "../3D-Engine/Resources/Shaders/Animation.frag");

    refreshScene();
}

void SimpleScene::initBones()
{
    _rootBone = std::make_unique<Bone>(0);
    _rootBone->addChildBone(1);
}

void SimpleScene::initWeights()
{
    std::vector<GLfloat> vertices = _geometry->vertices();
    std::vector<GLfloat> weights;

    for (unsigned int i = 0 ; i < vertices.size() ; i += 3)
    {
        glm::vec3 vertex(vertices[i], vertices[i + 1], vertices[i + 2]);
        auto weightRoot = (vertex.y + 1.5f) / 3.f;
        auto weightChild = 1 - weightRoot;

        weights.push_back(weightRoot);
        weights.push_back(weightChild);
    }

    _geometry->setWeights(weights);
}

void SimpleScene::updateMeshVertices()
{
    auto& initialPose = _geometry->initialPose();
    auto& bonesWeights = _geometry->weights();
    auto vertices = _geometry->vertices();
    auto rootBoneTransform = _rootBone->getTransformMatrix();
    auto childBoneTransform = _rootBone->firstChild()->getTransformMatrix();

    for(unsigned int i = 0; i < initialPose.size(); i += 3) {
        glm::vec4 initialVertex({ initialPose[i], initialPose[i+1], initialPose[i+2], 1.0 });
        auto vertexWeights = &bonesWeights[(i  / 3) * 2];

        auto weightedTransform = rootBoneTransform * vertexWeights[0] + childBoneTransform * vertexWeights[1];

        auto newVertex = weightedTransform * initialVertex;
        vertices[i] = newVertex.x; vertices[i + 1] = newVertex.y; vertices[i + 2] = newVertex.z;
    }

    _geometry->setVertices(vertices);
}

void SimpleScene::toggleDrawmode()
{
    _drawFill = !_drawFill;
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

long SimpleScene::getNumberOfTriangles()
{
    return _geometry->getNumberOfTriangles();
}

void SimpleScene::resize(int width, int height){
    _width = width;
    _height = height;
     glViewport(0, 0, _width, _height);

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

    useShader(SKYBOX_SHADER);
    glUniform1d(glGetUniformLocation(_programID, "skybox"), 0);
}

void SimpleScene::renderSkyBox()
{
    glDepthMask(GL_FALSE);
    useShader(SKYBOX_SHADER);

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

void SimpleScene::refreshGeometry()
{
    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, _geometry.get()->vertices().size() * sizeof (GLfloat), _geometry.get()->vertices().data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, _nbo);
    glBufferData(GL_ARRAY_BUFFER, _geometry.get()->normals().size() * sizeof (GLfloat), _geometry.get()->normals().data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(1);

    if (_animate)
    {
        glBindBuffer(GL_ARRAY_BUFFER, _wbo);
        glBufferData(GL_ARRAY_BUFFER, _geometry->weights().size() * sizeof(GLfloat), _geometry->weights().data(), GL_STATIC_READ);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 2, (GLvoid*)0);
        glEnableVertexAttribArray(2);
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _geometry.get()->indices().size() * sizeof (GLfloat), _geometry.get()->indices().data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void SimpleScene::initGeometryBuffers()
{
    glGenBuffers(1, &_vbo);
    glGenBuffers(1, &_nbo);
    glGenBuffers(1, &_ebo);
    glGenBuffers(1, &_wbo);
    glGenVertexArrays(1, &_vao);

    refreshGeometry();
}

void SimpleScene::renderScene()
{
    glBindVertexArray(_vao);
    glDrawElements(GL_TRIANGLES, _geometry.get()->indices().size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void SimpleScene::prepareShadows()
{
    glm::mat4 lightProjection, lightView;
    glm::mat4 lightSpaceMatrix;

    glm::vec3 lightPosition(-2.0f, 8.0f, -1.0f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    float nearPlane = 1.0f, farPlane = 7.5f;
    lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, nearPlane, farPlane);
    lightView = glm::lookAt(lightPosition, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
    lightSpaceMatrix = lightProjection * lightView;

    useShader(DEPTH_SHADOW_SHADER);
    glUniformMatrix4fv(glGetUniformLocation(_programID, "lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));

    glViewport(0, 0, 1024, 1024);
    glBindFramebuffer(GL_FRAMEBUFFER, _depthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);

    renderScene();

    glBindFramebuffer(GL_FRAMEBUFFER, QOpenGLContext::currentContext()->defaultFramebufferObject());

    glViewport(0, 0, _width, _height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    useShader(BASE_SHADOW_SHADER);
    // Vertex
    glUniformMatrix4fv(glGetUniformLocation(_programID, "model"), 1, GL_FALSE, glm::value_ptr(_model));
    glUniformMatrix4fv(glGetUniformLocation(_programID, "view"), 1, GL_FALSE, glm::value_ptr(_view));
    glUniformMatrix4fv(glGetUniformLocation(_programID, "projection"), 1, GL_FALSE, glm::value_ptr(_projection));
    glUniformMatrix4fv(glGetUniformLocation(_programID, "lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));

    // Fragment
    glUniform3f(glGetUniformLocation(_programID, "lightPos"), lightPosition[0], lightPosition[1], lightPosition[2]);
    glUniform3f(glGetUniformLocation(_programID, "viewPos"), _camera->position()[0], _camera->position()[1], _camera->position()[2]);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _depthMap);

    renderScene();
}

void SimpleScene::renderGeometry()
{
    if (_shadows && _drawFill)
    {
        // Compute scene depth in a texture
        prepareShadows();
    }
    else
    {
        useShader(BASE_SHADER);

        glUniformMatrix4fv(glGetUniformLocation(_programID, "model"), 1, GL_FALSE, glm::value_ptr(_model));
        glUniformMatrix4fv(glGetUniformLocation(_programID, "view"), 1, GL_FALSE, glm::value_ptr(_view));
        glUniformMatrix4fv(glGetUniformLocation(_programID, "projection"), 1, GL_FALSE, glm::value_ptr(_projection));

        renderScene();
    }
}

void SimpleScene::OpenGLDebug()
{
    GLenum err;
    if ((err = glGetError()) != GL_NO_ERROR) {
        std::string error;
        switch (err)
        {
        case GL_INVALID_OPERATION:
            error="INVALID_OPERATION";
            break;
        case GL_INVALID_ENUM:
            error="INVALID_ENUM";
            break;
        case GL_INVALID_VALUE:
            error="INVALID_VALUE";
            break;
        case GL_OUT_OF_MEMORY:
            error="OUT_OF_MEMORY";
            break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            error="INVALID_FRAMEBUFFER_OPERATION";
            break;
        default:
            error="UNKNOWN";
            break;
        }

        std::cerr << "OpenGL Error: " << error << std::endl;
    }
}

double SimpleScene::elapsedTime()
{
    auto time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - _start);
    return time.count() / 1000.f;
}

void SimpleScene::setAnimate(bool newAnimate)
{
    if (newAnimate != _animate)
    {
        _animate = newAnimate;
        _start = std::chrono::steady_clock::now();
        refreshScene();
    }
}

void SimpleScene::draw() {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    if (_drawFill)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    _view = _camera->viewmatrix();

    if (_hasSkybox)
    {
        renderSkyBox();
    }

    if (_animate)
    {
        // Needs to reinitialize geometry
        refreshGeometry();

        if (_shadows && _drawFill)
        {
            prepareShadows();
        }

        auto t = elapsedTime();

        _rootBone->firstChild()->setRotationZ(std::cos(t * 2) * 90.0f);
        _rootBone->setTranslationX(std::cos(t) * 2);
        _rootBone->setTranslationY(std::cos(t) * 3);
        _rootBone->firstChild()->setTranslationX(std::cos(t) * 2);

        _rootBone->firstChild()->setScale(glm::vec3(1., 1., 1.) * float(std::cos(t) * 0.5f + 1));

        useShader(ANIMATION_SHADER);
        glUniformMatrix4fv(glGetUniformLocation(_programID, "model"), 1, GL_FALSE, glm::value_ptr(_model));
        glUniformMatrix4fv(glGetUniformLocation(_programID, "view"), 1, GL_FALSE, glm::value_ptr(_view));
        glUniformMatrix4fv(glGetUniformLocation(_programID, "projection"), 1, GL_FALSE, glm::value_ptr(_projection));

        glUniformMatrix4fv(glGetUniformLocation(_programID, "rootTransform"), 1, GL_FALSE, glm::value_ptr(_rootBone->getTransformMatrix()));
        glUniformMatrix4fv(glGetUniformLocation(_programID, "childTransform"), 1, GL_FALSE, glm::value_ptr(_rootBone->firstChild()->getTransformMatrix()));

        updateMeshVertices();
        renderScene();
    }
    else
    {
        renderGeometry();
    }

   OpenGLDebug();
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

    glEnable(GL_DEPTH_TEST);

    initGeometryBuffers();
    if (_shadows)
    {
        glGenFramebuffers(1, &_depthMapFBO);

        // Create depth buffer texture
        glGenTextures(1, &_depthMap);
        glBindTexture(GL_TEXTURE_2D, _depthMap);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        float color[] = {1.0, 1.0, 1.0, 1.0};
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);

        glBindFramebuffer(GL_FRAMEBUFFER, _depthMapFBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _depthMap, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, QOpenGLContext::currentContext()->defaultFramebufferObject());

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            std::cerr << "Error while initializing depth frame buffer" << std::endl;
        }

        useShader(BASE_SHADOW_SHADER);
        glUniform1i(glGetUniformLocation(_programID, "shadowMap"), 0);
    }

    if (_hasSkybox)
    {
        initSkyBoxBuffers();
    }

    initBones();
    initWeights();
}

void SimpleScene::subdivideScene()
{
    _geometry->subdivide();
    refreshScene();
}

void SimpleScene::simplifyScene(long triangleTarget)
{
    _geometry->simplify(triangleTarget);
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
}

void SimpleScene::enableShadows(bool yes)
{
    if (_shadows != yes)
    {
        _shadows = yes;
        refreshScene();
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
