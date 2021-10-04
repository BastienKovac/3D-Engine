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
}

void MainWindow::on_getFirstPointBox_toggled(bool checked)
{
    ui->closseSplineBox->setChecked(false);
    ui->splinePlotter->spline()->setStartFromFirstPoint(checked);

    // Force refresh of the widget since the paint event since to be lost in transit somewhere
    ui->getFirstPointBox->setChecked(checked);
}

void MainWindow::on_finishAtLastPointBox_toggled(bool checked)
{
    ui->closseSplineBox->setChecked(false);
    ui->splinePlotter->spline()->setFinishAtLastPoint(checked);

    // Force refresh of the widget since the paint event since to be lost in transit somewhere
    ui->finishAtLastPointBox->setChecked(checked);
}

void MainWindow::on_closseSplineBox_toggled(bool checked)
{
    ui->getFirstPointBox->setChecked(false);
    ui->finishAtLastPointBox->setChecked(false);

    ui->splinePlotter->spline()->setCloseSpline(checked);

    // Force refresh of the widget since the paint event since to be lost in transit somewhere
    ui->closseSplineBox->setChecked(checked);
}

