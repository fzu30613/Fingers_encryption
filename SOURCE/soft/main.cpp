#include "mainwindow.h"
#include <QApplication>
#include <iostream>
using namespace std;



int main(int argc, char *argv[])
{

    QApplication app(argc, argv);
    MainWindow w;
    w.setFixedSize(350, 350);
    w.show();
    return app.exec();
}
