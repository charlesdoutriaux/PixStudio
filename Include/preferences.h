#include <QDockWidget>
#include <QtGui>
#include <QPathListWidget.h>

#ifndef QPREFS_H
#define QPREFS_H

class QPrefs : public QDockWidget
{
 Q_OBJECT
   public :
  QPrefs(const QString & title, QWidget * parent, Qt::WindowFlags flags);
  QLineEdit *outLineEdit;
  QLabel *distClusterLabel,*timeIntervalLabel,*fdl,*ldl;
  QPathListWidget *inPathList;
  QCheckBox *scanSubs,*useFirstTimeCheckBox,*useLastTimeCheckBox,*useTimeInterval,*useDistanceCluster;
  QDateEdit *firstDate,*lastDate;
  QTimeEdit *firstTime,*lastTime;
  QSlider *timeIntervalSlider,*distClusterSlider;
  QComboBox *timeIntervalUnits,*distClusterUnits;
  QToolButton *firstCalButton,*lastCalButton;
  bool needScan;
  public slots:
  void selectOutDir();
  void addInDir();
  void checkUseFirstTime(int state);
  void checkUseLastTime(int state);
  void timeIntervalValueChanged(int value);
  void distClusterValueChanged(int value);
  void useTimeIntervalChecked(int state);
  void useDistClusterChecked(int state);
  void checkedSubs(int state);
 private:
  void setupUi(void);
  QHBoxLayout *h,*h1,*h2,*h3,*h4,*h5;
  QVBoxLayout *vLayout,*v,*v1,*sepLayout,*pathsLayout;
  QLabel *prefsLabel,*l,*l1,*l2,*l3,*l4;
  QGroupBox *pathsGroupBox,*sepGroupBox;
  QToolButton *outPathButton,*inPathButton;
  QFrame *f;
  QFileDialog *dirDialog;
  QSpacerItem *sp,*sp1,*sp2;

};


#endif
