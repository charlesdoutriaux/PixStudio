#include <QtGui/QApplication>
#include <mainwindow.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":Icons/app.png"));
    MainWindow mainwin;
    mainwin.show();
    return a.exec();
}
