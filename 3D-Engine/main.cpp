#ifndef STB_IMAGE_IMPLEMENTATION

#define STB_IMAGE_IMPLEMENTATION
#include "3D/stb_image.h"

#endif

#include "UI/mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
