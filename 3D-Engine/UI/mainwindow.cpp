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


void MainWindow::on_loadFileButton_clicked()
{
    auto filePath = QFileDialog::getOpenFileName(this, tr("Open OBJ File"), "~", tr("OBJ Files (*.obj)"));
    if (filePath != nullptr)
    {
        ui->renderer->loadSceneFromFile(filePath.toStdString());
    }
}

void MainWindow::on_loadSceneButton_clicked()
{
    auto filePath = QFileDialog::getExistingDirectory(this, tr("Open scene directory"), "~");
    if (filePath != nullptr)
    {
        ui->renderer->loadSceneFromFile(filePath.toStdString());
    }
}
