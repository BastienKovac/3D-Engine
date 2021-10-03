#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtOpenGL>

#include <memory>

#include <Spline/bspline.h>

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
    void on_degreeSlider_valueChanged(int value);
    void on_getFirstPointBox_toggled(bool checked);
    void on_finishAtLastPointBox_toggled(bool checked);

    void on_closseSplineBox_toggled(bool checked);

private:
    Ui::MainWindow *ui;

    void refreshSplinePlot();
};
#endif // MAINWINDOW_H
