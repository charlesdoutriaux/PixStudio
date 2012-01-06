#include <QMainWindow>
#include <preferences.h>

#ifndef MAINWINDOW_H
#define MAINWINDOW_H


class MainWindow : public QMainWindow
{
  Q_OBJECT
    public: 
  MainWindow();
 private:
  void setupUi();
  void connectSignals();
  QTabWidget *tabs;
  QPrefs *prefs;
  QToolBar *tb;
  struct pix_entries *pix;
  public slots: 
  void refreshProjects();
  void renamePix();
  void newTabSelected(int tab);
  /* signals: */
};


#endif
