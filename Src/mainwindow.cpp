#include "Include/mainwindow.h"
#include <QtGui>

MainWindow::MainWindow() 
{
  QWidget *centralWidget = new QWidget();
  QVBoxLayout *v = new QVBoxLayout();
  QLabel *l = new QLabel("Hi");
  v->addWidget(l);
  centralWidget->setLayout(v);
  setCentralWidget(centralWidget);
  setWindowTitle("PixStudio");
}
