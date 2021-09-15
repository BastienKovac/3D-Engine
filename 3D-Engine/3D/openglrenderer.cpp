#include "openglrenderer.h"

OpenGLRenderer::OpenGLRenderer(QWidget *parent, int fps)
    : QOpenGLWidget(parent)
{
    if (fps == 0)
    {
        t_Timer = NULL;
    }
    else
    {
        int timerInterval = 1000 / fps;
        t_Timer = new QTimer(this);
        connect(t_Timer, SIGNAL(timeout()), this, SLOT(timeOutSlot()));
        t_Timer->start(timerInterval);
    }
}

void OpenGLRenderer::initializeGL()
{
    glShadeModel(GL_SMOOTH);

    float r, g, b, a = 1.0f;
    qColorToRGB(Qt::darkGray, r, g, b);
    glClearColor(r, g, b, a);
    glClearDepth(1.0f);

    glEnable(GL_DEPTH_TEST);
}

void OpenGLRenderer::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Model view matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
  //  gluLookAt(0.0, 0.0, -distance,
   //           0.0, 0.0, 0.0,
   //           0.0, 1.0, 0.0);

    glRotatef(xRot / 16.0f, 1.0f, 0.0f, 0.0f);
    glRotatef(yRot / 16.0f, 0.0f, 1.0f, 0.0f);
    glRotatef(zRot / 16.0f, 0.0f, 0.0f, 1.0f);

    // Projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    perspectiveGL(60.0f, 1.0 * width() / height(), 0.1f, 100.0f);
}

void OpenGLRenderer::resizeGL(int width, int height)
{
    if (height == 0)
    {
        height = 1;
    }

    glViewport(0, 0, width, height);
}

void OpenGLRenderer::keyPressEvent(QKeyEvent *keyEvent)
{
    switch (keyEvent->key())
    {
    case Qt::Key_Escape:
        close();
        break;
    }
}

void OpenGLRenderer::timeOutSlot()
{

}
