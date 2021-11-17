#include "mainwindow.h"
#include "ui_mainwindow.h"

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

    ui->vertexShaderPath->setText("BaseVertexShader.vert");
    ui->fragmentShaderPath->setText("BaseFragmentShader.frag");
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

void MainWindow::on_loadVertexShader_clicked()
{
    auto filePath = QFileDialog::getOpenFileName(this, tr("Get vertex shader file"), "~", tr("Vertex Shader (*.vert)"));
    if (filePath != nullptr)
    {
        std::string pathStr = filePath.toStdString();
        ui->renderer->loadShader(GL_VERTEX_SHADER, pathStr);

        ui->vertexShaderPath->setText(QString::fromStdString(pathStr.substr(pathStr.find_last_of("/") + 1)));
    }
}

void MainWindow::on_loadFragmentShader_clicked()
{
    auto filePath = QFileDialog::getOpenFileName(this, tr("Get fragment shader file"), "~", tr("Fragment Shader (*.frag)"));
    if (filePath != nullptr)
    {
        std::string pathStr = filePath.toStdString();
        ui->renderer->loadShader(GL_FRAGMENT_SHADER, pathStr);

        ui->fragmentShaderPath->setText(QString::fromStdString(pathStr.substr(pathStr.find_last_of("/") + 1)));
    }
}
