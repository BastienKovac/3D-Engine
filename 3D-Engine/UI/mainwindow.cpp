#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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
