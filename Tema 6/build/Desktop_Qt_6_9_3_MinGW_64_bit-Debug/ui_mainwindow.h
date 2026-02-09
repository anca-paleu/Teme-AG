/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QWidget *widget;
    QVBoxLayout *verticalLayout;
    QPushButton *btnLoad;
    QPushButton *btnFloyd;
    QPushButton *btnMST;
    QPushButton *btnTSP;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1500, 930);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        widget = new QWidget(centralwidget);
        widget->setObjectName("widget");
        widget->setGeometry(QRect(1380, 10, 116, 139));
        verticalLayout = new QVBoxLayout(widget);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        btnLoad = new QPushButton(widget);
        btnLoad->setObjectName("btnLoad");

        verticalLayout->addWidget(btnLoad);

        btnFloyd = new QPushButton(widget);
        btnFloyd->setObjectName("btnFloyd");

        verticalLayout->addWidget(btnFloyd);

        btnMST = new QPushButton(widget);
        btnMST->setObjectName("btnMST");

        verticalLayout->addWidget(btnMST);

        btnTSP = new QPushButton(widget);
        btnTSP->setObjectName("btnTSP");

        verticalLayout->addWidget(btnTSP);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1500, 25));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        btnLoad->setText(QCoreApplication::translate("MainWindow", "Incarca Date", nullptr));
        btnFloyd->setText(QCoreApplication::translate("MainWindow", "Floyd-Warshall", nullptr));
        btnMST->setText(QCoreApplication::translate("MainWindow", "MST Kruskal", nullptr));
        btnTSP->setText(QCoreApplication::translate("MainWindow", "Comis Voiajor", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
