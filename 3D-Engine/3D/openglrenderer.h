#ifndef OPENGLRENDERER_H
#define OPENGLRENDERER_H

#include <QColor>

#include <QtOpenGL>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>

#include <utils.h>

#include <QMatrix4x4>
#include <QVector3D>


class OpenGLRenderer : public QOpenGLWidget
{
    Q_OBJECT

public:
    OpenGLRenderer(QWidget *parent = nullptr, int fps = 60);

protected:
    virtual void initializeGL() override;
    virtual void paintGL() override;
    virtual void resizeGL(int width, int height) override;
    virtual void keyPressEvent(QKeyEvent *keyEvent) override;

    virtual void wheelEvent(QWheelEvent *wheelEvent) override;
    virtual void mousePressEvent(QMouseEvent *mouseEvent) override;
    virtual void mouseMoveEvent(QMouseEvent *mouseEvent) override;

public slots:
    virtual void updateGL();

private:
    QTimer *t_Timer;

    QPointF lastPosition;

    float distance = 5.0f;
    float xRot = 0.0f;
    float yRot = 0.0f;
    float zRot = 0.0f;

    void perspectiveGL(GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar)
    {
        GLdouble fW, fH;

        fH = tan(fovY / 360 * DOUBLE_PI) * zNear;
        fW = fH * aspect;

        glFrustum(-fW, fW, -fH, fH, zNear, zFar);
    }

    void rotateBy(int x, int y, int z)
    {
        xRot += x;
        yRot += y;
        zRot += z;
    }

};

#endif // OPENGLRENDERER_H
