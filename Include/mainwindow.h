#include <QMainWindow>
#include <preferences.h>

#ifndef MAINWINDOW_H
#define MAINWINDOW_H


class MainWindow : public QMainWindow
{
  Q_OBJECT
      public: 
  MainWindow();
  void setupUi();
 private:
  QTabWidget *tabs;
  QPrefs *prefs;
  /*  slots: */
  /* signals: */
};


#endif
