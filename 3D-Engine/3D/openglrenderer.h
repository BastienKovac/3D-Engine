#ifndef OPENGLRENDERER_H
#define OPENGLRENDERER_H

#include <QColor>

#include <QtOpenGL>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>

#include <utils.h>

#include <QMatrix4x4>
#include <QVector3D>

#include <3D/Camera/camera.h>
#include <3D/openglbase.h>
#include <3D/Display/simplescene.h>

class OpenGLRenderer : public QOpenGLWidget, protected QOpenGLFunctions_4_1_Core
{
    Q_OBJECT

public:
    OpenGLRenderer(QWidget *parent = nullptr, int fps = 60);

    void activateDemo(unsigned int numDemo);

protected:
    virtual void initializeGL() override;
    virtual void paintGL() override;
    virtual void resizeGL(int width, int height) override;

    virtual void keyPressEvent(QKeyEvent *keyEvent) override;
    virtual void mousePressEvent(QMouseEvent *mouseEvent) override;
    virtual void mouseMoveEvent(QMouseEvent *mouseEvent) override;

public slots:
    virtual void updateGL();
    void cleanup();

private:
    // What's being drawn
    std::unique_ptr<OpenGLBase> _scene;

    // Stored demos
    using DemoConstructors=std::function<OpenGLBase*(int, int)>;
    std::vector<DemoConstructors> _demoConstructors;

    QTimer *t_Timer;

};

#endif // OPENGLRENDERER_H
