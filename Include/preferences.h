#include <QFrame>
#include <QtGui>
#include <QPathListWidget.h>

#ifndef QPREFS_H
#define QPREFS_H

class QPrefs : public QFrame
{
 Q_OBJECT
   public :
  QPrefs();
  public slots:
  void selectOutDir();
  void addInDir();
  void checkUseFirstTime(int state);
  void checkUseLastTime(int state);
  void timeIntervalValueChanged(int value);
  void distClusterValueChanged(int value);
  void useTimeIntervalChecked(int state);
  void useDistClusterChecked(int state);
 private:
  void setupUi(void);
  QLabel *distClusterLabel,*timeIntervalLabel,*fdl,*ldl;
  QLineEdit *outLineEdit;
  QPathListWidget *inPathList;
  QCheckBox *scanSubs,*useFirstTimeCheckBox,*useLastTimeCheckBox,*useTimeInterval,*useDistanceCluster;
  QDateEdit *firstDate,*lastDate;
  QTimeEdit *firstTime,*lastTime;
  QSlider *timeIntervalSlider,*distClusterSlider;
  QComboBox *timeIntervalUnits,*distClusterUnits;
  QToolButton *firstCalButton,*lastCalButton;
};


#endif
