#include <QtGui/QApplication>
#include "Include/mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow mainwin;
    mainwin.show();
    return a.exec();
}
