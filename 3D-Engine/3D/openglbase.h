#ifndef OPENGLBASE_H
#define OPENGLBASE_H

#include <vector>
#include <iostream>
#include <string>

#include <QOpenGLFunctions_4_1_Core>
#include <QString>

class OpenGLBase : public QOpenGLFunctions_4_1_Core
{
public:
    explicit OpenGLBase(int width, int height);
    virtual ~OpenGLBase();

    virtual void resize(int width, int height);
    virtual void draw();

    virtual void mouseclick(int button, float xpos, float ypos);
    virtual void mousemove(float xpos, float ypos);
    virtual void mousewheel(float delta);
    virtual void keyboardmove(int key, double time);
    virtual bool keyboard(unsigned char k);

    virtual void loadSceneFromFile(std::string fileName);
    virtual void subdivideScene();

    virtual void loadSkybox(std::string path);

    void toggleDrawmode();

protected:
    int _width;
    int _height;

private:
    // Rendering mode (true is filled, false is wireframed
    bool _drawFill;

};

#endif // OPENGLBASE_H
