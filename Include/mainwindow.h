#include <QMainWindow>
#include <preferences.h>

#ifndef MAINWINDOW_H
#define MAINWINDOW_H


class MainWindow : public QMainWindow
{
  Q_OBJECT
    public: 
  MainWindow();
 public:
  QPrefs *prefs;
  QTabWidget *tabs;
 private:
  void setupUi();
  void connectSignals();
  QToolBar *tb;
  struct pix_entries *pix;
  public slots: 
  void refreshProjects();
  void renamePix();
  void newTabSelected(int tab);
  void closeATab(int i);
  /* signals: */
};


#endif
