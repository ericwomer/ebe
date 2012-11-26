#include <QApplication>
#include <QPalette>
#include <QColor>
#include <QFont>
#include <stdio.h>

#include "mainwindow.h"

QApplication *qapp;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    qapp = &app;
    QPalette pal = app.palette();
    pal.setColor(QPalette::Window, QColor(218,218,228));
    app.setPalette(pal);
    MainWindow mainWin;
    mainWin.resize(1200,900);
    mainWin.show();
    return app.exec();
}
