#include "mainwindow.h"
#include <QApplication>
#include <QLabel>

int main(int argc, char *argv[])
{
    setlocale(LC_ALL, "RUS");
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}




