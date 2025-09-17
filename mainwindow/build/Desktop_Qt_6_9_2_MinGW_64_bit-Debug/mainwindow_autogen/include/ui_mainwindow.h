/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QPushButton *chooseFileButton;
    QLineEdit *filePathEdit;
    QLineEdit *keyEdit;
    QPushButton *startButton;
    QProgressBar *progressBar;
    QCheckBox *checkBox_SursFile;
    QPushButton *ButtonExit;
    QLabel *lable_text;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(743, 456);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        chooseFileButton = new QPushButton(centralwidget);
        chooseFileButton->setObjectName("chooseFileButton");
        chooseFileButton->setGeometry(QRect(80, 30, 80, 24));
        filePathEdit = new QLineEdit(centralwidget);
        filePathEdit->setObjectName("filePathEdit");
        filePathEdit->setGeometry(QRect(180, 30, 113, 24));
        keyEdit = new QLineEdit(centralwidget);
        keyEdit->setObjectName("keyEdit");
        keyEdit->setGeometry(QRect(180, 100, 113, 24));
        startButton = new QPushButton(centralwidget);
        startButton->setObjectName("startButton");
        startButton->setGeometry(QRect(330, 340, 80, 24));
        progressBar = new QProgressBar(centralwidget);
        progressBar->setObjectName("progressBar");
        progressBar->setGeometry(QRect(70, 220, 581, 23));
        progressBar->setValue(0);
        checkBox_SursFile = new QCheckBox(centralwidget);
        checkBox_SursFile->setObjectName("checkBox_SursFile");
        checkBox_SursFile->setGeometry(QRect(80, 70, 77, 21));
        ButtonExit = new QPushButton(centralwidget);
        ButtonExit->setObjectName("ButtonExit");
        ButtonExit->setGeometry(QRect(680, 0, 31, 21));
        QFont font;
        font.setPointSize(10);
        ButtonExit->setFont(font);
        lable_text = new QLabel(centralwidget);
        lable_text->setObjectName("lable_text");
        lable_text->setGeometry(QRect(20, 10, 49, 16));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 743, 21));
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
        chooseFileButton->setText(QCoreApplication::translate("MainWindow", "PushButton", nullptr));
        startButton->setText(QCoreApplication::translate("MainWindow", "PushButton", nullptr));
        checkBox_SursFile->setText(QCoreApplication::translate("MainWindow", "CheckBox", nullptr));
        ButtonExit->setText(QCoreApplication::translate("MainWindow", "PushButton", nullptr));
        lable_text->setText(QCoreApplication::translate("MainWindow", "TextLabel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
