#include "openglrenderer.h"

#include <iostream>

OpenGLRenderer::OpenGLRenderer(QWidget *parent, int fps) : QOpenGLWidget(parent)
{
    if (fps == 0)
    {
        t_Timer = NULL;
    }
    else
    {
        int timerInterval = 1000 / fps;
        t_Timer = new QTimer(this);
        connect(t_Timer, SIGNAL(timeout()), this, SLOT(updateGL()));
        t_Timer->start(timerInterval);
    }
}

void OpenGLRenderer::cleanup() {
    _scene.reset(nullptr);
}

void OpenGLRenderer::initializeGL()
{
    connect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &OpenGLRenderer::cleanup);

    if (!initializeOpenGLFunctions())
    {
        QMessageBox::critical(this, "OpenGL initialization error", "MyOpenGLWidget::initializeGL() : Unable to initialize OpenGL functions");
        exit(1);
    }

    _scene.reset(new SimpleScene(width(), height()));
}

void OpenGLRenderer::paintGL()
{
    _scene->draw();
    glFinish();
}

void OpenGLRenderer::resizeGL(int width, int height)
{
    _scene->resize(width, height);
}

void OpenGLRenderer::keyPressEvent(QKeyEvent *keyEvent)
{
    switch (keyEvent->key())
    {
    case Qt::Key_Left:
    case Qt::Key_Up:
    case Qt::Key_Right:
    case Qt::Key_Down:
        // Move keys
        _scene->keyboardmove(keyEvent->key() - Qt::Key_Left, 1. / 100);
        update();
        break;
    case Qt::Key_W:
        _scene->toggleDrawmode();
        update();
        break;
    case Qt::Key_Plus:
        makeCurrent();
        _scene->subdivideScene();
        update();
        doneCurrent();

        emit geometryChanged();
        break;
    default :
        // Other keys are transmitted to the scene, opening an OpenGL context
        if (_scene->keyboard(keyEvent->text().toStdString()[0]))
        {
            update();
        }
    }
}

void OpenGLRenderer::mousePressEvent(QMouseEvent *mouseEvent) {
    // Force focus to OpenGL renderer
    setFocus();

    // buttons are 0(left), 1(right) to 2(middle)
    int b;
    Qt::MouseButton button=mouseEvent->button();
    if (button & Qt::LeftButton)
    {
        if ((mouseEvent->modifiers() & Qt::ControlModifier))
        {
            b = 2;
        }
        else
        {
            b = 0;
        }
    }
    else if (button & Qt::RightButton)
    {
        b = 1;
    }
    else if (button & Qt::MiddleButton)
    {
        b = 2;
    }
    else
    {
        b = 3;
    }
    _scene->mouseclick(b, mouseEvent->x(), mouseEvent->y());
}

void OpenGLRenderer::mouseMoveEvent(QMouseEvent *mouseEvent) {
    _scene->mousemove(mouseEvent->x(), mouseEvent->y());
    update();
}

void OpenGLRenderer::loadSceneFromFile(std::string fileName)
{
    makeCurrent();
    _scene->loadSceneFromFile(fileName);
    doneCurrent();
}

void OpenGLRenderer::loadSkybox(std::string path)
{
    makeCurrent();
    _scene->loadSkybox(path);
    doneCurrent();
}

void OpenGLRenderer::simplifyScene(long triangleTarget)
{
    makeCurrent();
    _scene->simplifyScene(triangleTarget);
    doneCurrent();
}

long OpenGLRenderer::getNumberOfTriangles()
{
    return _scene->getNumberOfTriangles();
}

void OpenGLRenderer::enableShadows(bool yes)
{
    makeCurrent();
    _scene->enableShadows(yes);
    doneCurrent();
}


void OpenGLRenderer::updateGL()
{
    update();
}
