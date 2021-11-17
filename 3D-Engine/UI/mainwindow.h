#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtOpenGL>
#include <QFileDialog>

#include <memory>
#include <iostream>
#include <GL/gl.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_loadFileButton_clicked();

    void on_loadVertexShader_clicked();

    void on_loadFragmentShader_clicked();

private:
    Ui::MainWindow *ui;

};
#endif // MAINWINDOW_H
