#include "Include/mainwindow.h"
#include "Include/preferences.h"
#include <QtGui>
#include <Qt>

MainWindow::MainWindow() {
  setupUi();
}

void MainWindow::setupUi() {
  QSplitter *centralWidget = new QSplitter(Qt::Horizontal);
  QPrefs *prefs = new QPrefs();
  centralWidget->addWidget(prefs);
  QTabWidget *tabs = new QTabWidget();
  centralWidget->addWidget(tabs);
  setCentralWidget(centralWidget);
  setWindowTitle("PixStudio");
}

