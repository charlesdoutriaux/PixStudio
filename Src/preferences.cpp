#include <QtGui>
#include <Qt>
#include "Include/preferences.h"

QPrefs::QPrefs() {
  setupUi();
};

void QPrefs::setupUi() {
  QVBoxLayout *vLayout = new QVBoxLayout();
  QLabel *prefsLabel = new QLabel("Preferences");
  vLayout->addWidget(prefsLabel);

  /* Paths Related Area */
  QGroupBox *pathsGroupBox = new QGroupBox();
  pathsGroupBox->setTitle("Paths");
  /* GroupBox Layout */
  QVBoxLayout *pathsLayout = new QVBoxLayout();

  /* Output Path */
  QHBoxLayout *h = new QHBoxLayout();
  QLabel *l = new QLabel("Out");
  h->addWidget(l);
  QLineEdit *outLineEdit = new QLineEdit();
  h->addWidget(outLineEdit);
  QToolButton *outPathButton = new QToolButton();
  h->addWidget(outPathButton);
  pathsLayout->addLayout(h);

  /* Input Paths */
  QVBoxLayout *v = new QVBoxLayout();
  h = new QHBoxLayout();
  l = new QLabel("In");
  h->addWidget(l);
  QToolButton *inPathButton = new QToolButton();
  h->addWidget(inPathButton);
  v->addLayout(h);
  QListWidget *inPathList = new QListWidget();
  v->addWidget(inPathList);
  pathsLayout->addLayout(v);

  /* Setslayout for Paths GroupBox */
  pathsGroupBox->setLayout(pathsLayout);

  /*Add GroupBox for Paths to Prefs Widget Layout */
  vLayout->addWidget(pathsGroupBox);

  /* Picture Separation Settings */
  QGroupBox *sepGroupBox = new QGroupBox();
  sepGroupBox->setTitle("Pictures Grouping Selection Criteria");
  /* Separation Layout */
  QVBoxLayout *sepLayout = new QVBoxLayout();
  /* First Time */
  h = new QHBoxLayout();
  QCheckBox *useFirstTimeCheckBox = new QCheckBox();
  h->addWidget(useFirstTimeCheckBox);
  l = new QLabel("First Time");
  h->addWidget(l);
  QDateEdit *firstDate = new QDateEdit();
  h->addWidget(firstDate);
  QToolButton *firstCalButton = new QToolButton();
  h->addWidget(firstCalButton);
  QTimeEdit *firstTime = new QTimeEdit();
  h->addWidget(firstTime);
  /* Add First Date to Sep Layout */
  sepLayout->addLayout(h);
  /* Second Time */
  h = new QHBoxLayout();
  QCheckBox *useLastTimeCheckBox = new QCheckBox();
  h->addWidget(useLastTimeCheckBox);
  l = new QLabel("Last Time");
  h->addWidget(l);
  QDateEdit *lastDate = new QDateEdit();
  h->addWidget(lastDate);
  QToolButton *lastCalButton = new QToolButton();
  h->addWidget(lastCalButton);
  QTimeEdit *lastTime = new QTimeEdit();
  h->addWidget(lastTime);
  /* Add Last Date to Sep Layout */
  sepLayout->addLayout(h);

  /* Time Interval */
  h = new QHBoxLayout();
  l = new QLabel("Time Interval");
  h->addWidget(l);
  QLabel *timeIntervalLabel = new QLabel("30");
  h->addWidget(timeIntervalLabel);
  QSlider *timeIntervalSlider = new QSlider(Qt::Horizontal);
  h->addWidget(timeIntervalSlider);
  QComboBox *timeIntervalUnits = new QComboBox();
  timeIntervalUnits->addItem("Days");
  timeIntervalUnits->addItem("Hours");
  timeIntervalUnits->addItem("Minutes");
  timeIntervalUnits->addItem("Seconds");
  timeIntervalUnits->setCurrentIndex(2);
  h->addWidget(timeIntervalUnits);
  /* Add Time Interval to Sep Layout */
  sepLayout->addLayout(h);

  /* Distance */
  h = new QHBoxLayout();
  l = new QLabel("Distance Cluster");
  h->addWidget(l);
  QLabel *distClusterLabel = new QLabel("0");
  h->addWidget(distClusterLabel);
  QSlider *distClusterSlider = new QSlider(Qt::Horizontal);
  h->addWidget(distClusterSlider);
  QComboBox *distClusterUnits = new QComboBox();
  distClusterUnits->addItem("miles");
  distClusterUnits->addItem("km");
  distClusterUnits->addItem("feet");
  distClusterUnits->addItem("m");
  distClusterUnits->setCurrentIndex(0);
  h->addWidget(distClusterUnits);
  /* Add Distamce Cluster to Sep Layout */
  sepLayout->addLayout(h);


  /* */
  sepGroupBox->setLayout(sepLayout);
  vLayout->addWidget(sepGroupBox);
  /* Sets Prefs Widget Layout */
  setLayout(vLayout);
}

