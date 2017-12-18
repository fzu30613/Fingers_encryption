#include "mainwindow.h"
#include <QApplication>
#include <iostream>
#include "server.h"
using namespace std;



int main(int argc, char *argv[])
{

    QApplication app(argc, argv);
    MainWindow w;
    w.setFixedSize(400, 100);
    w.show();
    return app.exec();
}
