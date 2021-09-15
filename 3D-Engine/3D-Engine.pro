QT       += core gui opengl openglwidgets charts
LIBS     += -lOpengl32 -lglu32
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Spline/bspline.cpp \
    UI/bsplinechart.cpp \
    main.cpp \
    UI/mainwindow.cpp \
    3D/openglrenderer.cpp

HEADERS += \
    Spline/bspline.h \
    UI/bsplinechart.h \
    UI/mainwindow.h \
    3D/openglrenderer.h \
    utils.h

FORMS += \
    UI/mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
