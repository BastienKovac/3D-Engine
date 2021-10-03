#include "openglrenderer.h"

#include <iostream>
#include <3D/openglbase.h>

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

    _demoConstructors.push_back( [](int width, int height)->OpenGLBase*{
        return new OpenGLGeometry(width, height);
    });
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

    _scene.reset(_demoConstructors[0](width(), height()));
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
    case Qt::Key_0:
    case Qt::Key_1:
    case Qt::Key_2:
    case Qt::Key_3:
    case Qt::Key_4:
    case Qt::Key_5:
    case Qt::Key_6:
    case Qt::Key_7:
    case Qt::Key_8:
    case Qt::Key_9:
        // Demo keys
        activateDemo(keyEvent->key() - Qt::Key_0);
        break;
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
    default :
        // Other keys are transmitted to the scene
        if (_scene->keyboard(keyEvent->text().toStdString()[0]))
        {
            update();
        }
    }
}

void OpenGLRenderer::mousePressEvent(QMouseEvent *mouseEvent) {
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

void OpenGLRenderer::activateDemo(unsigned int numDemo) {
    if (numDemo < _demoConstructors.size()) {
        std::cout << "Activating demo " << numDemo << " : " << std::endl;
        makeCurrent();
        _scene.reset(_demoConstructors[numDemo](width(), height()));
        doneCurrent();
        update();
    }
}


void OpenGLRenderer::updateGL()
{
    update();
}
