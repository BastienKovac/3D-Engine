#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    QSurfaceFormat format;
    format.setVersion(4, 1);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setDepthBufferSize(24);
    QSurfaceFormat::setDefaultFormat(format);

    ui->setupUi(this);

    ui->renderer->resize(ui->renderer->sizeHint());
    ui->renderer->setFocus();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_degreeSlider_valueChanged(int value)
{
    ui->splinePlotter->spline()->setDegree(value);
    refreshSplinePlot();
}

void MainWindow::on_getFirstPointBox_toggled(bool checked)
{
    ui->splinePlotter->spline()->setStartFromFirstPoint(checked);
    refreshSplinePlot();
}


void MainWindow::on_finishAtLastPointBox_toggled(bool checked)
{
    ui->splinePlotter->spline()->setFinishAtLastPoint(checked);
    refreshSplinePlot();
}

void MainWindow::refreshSplinePlot()
{
    ui->splinePlotter->refreshBSpline();
}

void MainWindow::on_closseSplineBox_toggled(bool checked)
{
    ui->splinePlotter->spline()->setCloseSpline(checked);
    refreshSplinePlot();
}

