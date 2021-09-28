/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <3D/openglrenderer.h>
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <UI/bsplinechart.h>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QTabWidget *configurationTab;
    QWidget *bSplineTab_2;
    QFormLayout *formLayout;
    QWidget *widget;
    QFormLayout *formLayout_3;
    QLabel *label;
    QSlider *degreeSlider;
    QCheckBox *getFirstPointBox;
    QCheckBox *finishAtLastPointBox;
    BSplineChartView *splinePlotter;
    QWidget *importTab_2;
    QFormLayout *formLayout_2;
    QPushButton *loadFileButton;
    OpenGLRenderer *renderer;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1600, 1200);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        configurationTab = new QTabWidget(centralwidget);
        configurationTab->setObjectName(QStringLiteral("configurationTab"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(configurationTab->sizePolicy().hasHeightForWidth());
        configurationTab->setSizePolicy(sizePolicy);
        configurationTab->setMinimumSize(QSize(782, 400));
        bSplineTab_2 = new QWidget();
        bSplineTab_2->setObjectName(QStringLiteral("bSplineTab_2"));
        formLayout = new QFormLayout(bSplineTab_2);
        formLayout->setObjectName(QStringLiteral("formLayout"));
        widget = new QWidget(bSplineTab_2);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setMinimumSize(QSize(100, 100));
        formLayout_3 = new QFormLayout(widget);
        formLayout_3->setObjectName(QStringLiteral("formLayout_3"));
        label = new QLabel(widget);
        label->setObjectName(QStringLiteral("label"));

        formLayout_3->setWidget(0, QFormLayout::LabelRole, label);

        degreeSlider = new QSlider(widget);
        degreeSlider->setObjectName(QStringLiteral("degreeSlider"));
        degreeSlider->setMinimum(1);
        degreeSlider->setMaximum(6);
        degreeSlider->setValue(3);
        degreeSlider->setOrientation(Qt::Horizontal);
        degreeSlider->setInvertedAppearance(false);
        degreeSlider->setInvertedControls(false);
        degreeSlider->setTickPosition(QSlider::TicksBelow);
        degreeSlider->setTickInterval(1);

        formLayout_3->setWidget(0, QFormLayout::FieldRole, degreeSlider);

        getFirstPointBox = new QCheckBox(widget);
        getFirstPointBox->setObjectName(QStringLiteral("getFirstPointBox"));

        formLayout_3->setWidget(1, QFormLayout::LabelRole, getFirstPointBox);

        finishAtLastPointBox = new QCheckBox(widget);
        finishAtLastPointBox->setObjectName(QStringLiteral("finishAtLastPointBox"));

        formLayout_3->setWidget(2, QFormLayout::LabelRole, finishAtLastPointBox);


        formLayout->setWidget(0, QFormLayout::LabelRole, widget);

        splinePlotter = new BSplineChartView(bSplineTab_2);
        splinePlotter->setObjectName(QStringLiteral("splinePlotter"));

        formLayout->setWidget(0, QFormLayout::FieldRole, splinePlotter);

        configurationTab->addTab(bSplineTab_2, QString());
        importTab_2 = new QWidget();
        importTab_2->setObjectName(QStringLiteral("importTab_2"));
        formLayout_2 = new QFormLayout(importTab_2);
        formLayout_2->setObjectName(QStringLiteral("formLayout_2"));
        loadFileButton = new QPushButton(importTab_2);
        loadFileButton->setObjectName(QStringLiteral("loadFileButton"));

        formLayout_2->setWidget(0, QFormLayout::LabelRole, loadFileButton);

        configurationTab->addTab(importTab_2, QString());

        verticalLayout->addWidget(configurationTab);

        renderer = new OpenGLRenderer(centralwidget);
        renderer->setObjectName(QStringLiteral("renderer"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(renderer->sizePolicy().hasHeightForWidth());
        renderer->setSizePolicy(sizePolicy1);
        renderer->setMinimumSize(QSize(0, 300));

        verticalLayout->addWidget(renderer);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 1600, 20));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        configurationTab->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "3D-Engine", Q_NULLPTR));
        label->setText(QApplication::translate("MainWindow", "B-Spline Degree", Q_NULLPTR));
        getFirstPointBox->setText(QApplication::translate("MainWindow", "Snap to first point", Q_NULLPTR));
        finishAtLastPointBox->setText(QApplication::translate("MainWindow", "Snap to last point", Q_NULLPTR));
        configurationTab->setTabText(configurationTab->indexOf(bSplineTab_2), QApplication::translate("MainWindow", "Generate B-Spline", Q_NULLPTR));
        loadFileButton->setText(QApplication::translate("MainWindow", "Load File", Q_NULLPTR));
        configurationTab->setTabText(configurationTab->indexOf(importTab_2), QApplication::translate("MainWindow", "Import Geometry", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
