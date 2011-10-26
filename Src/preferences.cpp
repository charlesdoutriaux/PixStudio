#include <QtGui>
#include <Qt>
#include "Include/preferences.h"

QPrefs::QPrefs() {
  setupUi();
};

void QPrefs::setupUi(void) {
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
  this->outLineEdit = new QLineEdit();
  h->addWidget(this->outLineEdit);
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
  this->inPathList = new QPathListWidget();
  v->addWidget(this->inPathList);
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
  this->useFirstTimeCheckBox = new QCheckBox();
  h->addWidget(this->useFirstTimeCheckBox);
  l = new QLabel("First Time");
  h->addWidget(l);
  this->firstDate = new QDateEdit();
  this->firstDate->setEnabled(false);
  h->addWidget(this->firstDate);
  this->firstCalButton = new QToolButton();
  this->firstCalButton->setEnabled(false);
  h->addWidget(firstCalButton);
  this->firstTime = new QTimeEdit();
  this->firstTime->setEnabled(false);
  h->addWidget(this->firstTime);
  /* Add First Date to Sep Layout */
  sepLayout->addLayout(h);
  /* Second Time */
  h = new QHBoxLayout();
  this->useLastTimeCheckBox = new QCheckBox();
  h->addWidget(this->useLastTimeCheckBox);
  l = new QLabel("Last Time");
  h->addWidget(l);
  this->lastDate = new QDateEdit();
  this->lastDate->setEnabled(false);
  h->addWidget(this->lastDate);
  this->lastCalButton = new QToolButton();
  this->lastCalButton->setEnabled(false);
  h->addWidget(lastCalButton);
  this->lastTime = new QTimeEdit();
  this->lastTime->setEnabled(false);
  h->addWidget(this->lastTime);
  /* Add Last Date to Sep Layout */
  sepLayout->addLayout(h);

  /* Time Interval */
  h = new QHBoxLayout();
  this->useTimeInterval = new QCheckBox();
  this->useTimeInterval->setCheckState(Qt::Checked);
  h->addWidget(this->useTimeInterval);
  l = new QLabel("Time Interval");
  h->addWidget(l);
  this->timeIntervalLabel = new QLabel("30");
  h->addWidget(this->timeIntervalLabel);
  this->timeIntervalSlider = new QSlider(Qt::Horizontal);
  h->addWidget(this->timeIntervalSlider);
  this->timeIntervalUnits = new QComboBox();
  this->timeIntervalUnits->addItem("Days");
  this->timeIntervalUnits->addItem("Hours");
  this->timeIntervalUnits->addItem("Minutes");
  this->timeIntervalUnits->addItem("Seconds");
  this->timeIntervalUnits->setCurrentIndex(2);
  h->addWidget(timeIntervalUnits);
  /* Add Time Interval to Sep Layout */
  sepLayout->addLayout(h);

  /* Distance */
  h = new QHBoxLayout();
  this->useDistanceCluster = new QCheckBox();
  this->useDistanceCluster->setCheckState(Qt::Checked);
  h->addWidget(this->useDistanceCluster);
  l = new QLabel("Distance Cluster");
  h->addWidget(l);
  this->distClusterLabel = new QLabel("0");
  h->addWidget(this->distClusterLabel);
  this->distClusterSlider = new QSlider(Qt::Horizontal);
  h->addWidget(this->distClusterSlider);
  this->distClusterUnits = new QComboBox();
  this->distClusterUnits->addItem("miles");
  this->distClusterUnits->addItem("km");
  this->distClusterUnits->addItem("feet");
  this->distClusterUnits->addItem("m");
  this->distClusterUnits->setCurrentIndex(0);
  h->addWidget(this->distClusterUnits);
  /* Add Distamce Cluster to Sep Layout */
  sepLayout->addLayout(h);


  /* */
  sepGroupBox->setLayout(sepLayout);
  vLayout->addWidget(sepGroupBox);
  /* Sets Prefs Widget Layout */
  setLayout(vLayout);

  /* Ok nowconnect signals */

  connect(outPathButton,SIGNAL(clicked()),this,SLOT(selectOutDir()));
  connect(inPathButton,SIGNAL(clicked()),this,SLOT(addInDir()));
  connect(this->useFirstTimeCheckBox,SIGNAL(stateChanged(int)),this,SLOT(checkUseFirstTime()));
  connect(this->useLastTimeCheckBox,SIGNAL(stateChanged(int)),this,SLOT(checkUseLasTime()));

}

void QPrefs::selectOutDir() {
  QFileDialog *dirDialog = new QFileDialog();
  QString outDir = dirDialog->getExistingDirectory(this,"Choose Output Root Directory");
  this->outLineEdit->setText(outDir);
}
void QPrefs::addInDir() {
  int i;
  bool unique;
  QListWidgetItem *item;
  QFileDialog *dirDialog = new QFileDialog();
  QString inDir = dirDialog->getExistingDirectory(this,"Choose Output Root Directory");
  unique = true;
  for (i=0;i< this->inPathList->count();i++) {
    item = this->inPathList->item(i);
    if (item->text().operator==(inDir)) {
      unique = false;
    }
  }
  if (unique) this->inPathList->addItem(inDir);
}
void QPrefs::checkUseFirstTime(int state) {
}
void QPrefs::checkUseLastTime(int state) {
}


