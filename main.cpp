#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
// #ifdef Q_OS_WINDOWS
//     QFont font("Arial");
// #else
//     QFont font("Ubuntu");
// #endif
//     font.setPointSize(9);
//     w.setFont(font);
    w.show();
    return a.exec();
}
