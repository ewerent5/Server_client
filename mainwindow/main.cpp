#include "mainwindow.h"

#include <QApplication>
#include <QMenuBar>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.resize(600, 340);
    w.setStyleSheet("background-color:rgb(21, 23, 25); color: white");
    w.show();
    return a.exec();
}
