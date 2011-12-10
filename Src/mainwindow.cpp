#include <Qt>
#include <QtGui>
#include <mainwindow.h>
#include <preferences.h>

MainWindow::MainWindow() {
  setupUi();
}

void MainWindow::setupUi() {
  this->prefs = new QPrefs();
  addDockWidget(Qt::LeftDockWidgetArea,this->prefs);
  //centralWidget->addWidget(prefs);
  //printf("Ok dist label is: %s\n",(const char *) prefs->distClusterLabel->text().toAscii());
  this->tabs = new QTabWidget;
  this->setCentralWidget(this->tabs);
  setWindowTitle("PixStudio");
  this->tb = new QToolBar();
  addToolBar(this->tb);
  QMenuBar *menu = new QMenuBar();
  menu->addMenu(tr("&View"));
}

