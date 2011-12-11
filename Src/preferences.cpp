#include <QtGui>
#include <Qt>
#include <preferences.h>
//#include <qtthumbwheel.h>

QPrefs::QPrefs(const QString & title, QWidget * parent = 0, Qt::WindowFlags flags = 0) {
  setParent(parent);
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
  QSpacerItem *sp = new QSpacerItem(40,10,QSizePolicy::Expanding,QSizePolicy::Minimum);
  h->addItem(sp);
  this->scanSubs = new QCheckBox();
  h->addWidget(this->scanSubs);
  l = new QLabel("scan subdirectories?");
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
  this->fdl = new QLabel("First Time");
  this->fdl->setEnabled(false);
  h->addWidget(this->fdl);
  h->addItem(sp);
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
  this->ldl = new QLabel("Last Time");
  this->ldl->setEnabled(false);
  h->addWidget(this->ldl);
  h->addItem(sp);
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
  this->useTimeInterval->setEnabled(false); // for now
  h->addWidget(this->useTimeInterval);
  l = new QLabel("Time Interval");
  h->addWidget(l);
  this->timeIntervalLabel = new QLabel("30");
  h->addWidget(this->timeIntervalLabel);
  this->timeIntervalSlider = new QSlider(Qt::Horizontal);
  this->timeIntervalSlider->setMaximum(100);
  this->timeIntervalSlider->setMinimum(1);
  this->timeIntervalSlider->setValue(30);
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
  this->useDistanceCluster->setCheckState(Qt::Unchecked);
  this->useDistanceCluster->setEnabled(false);
  h->addWidget(this->useDistanceCluster);
  l = new QLabel("Distance Cluster");
  l->setEnabled(false);
  h->addWidget(l);
  this->distClusterLabel = new QLabel("1");
  this->distClusterLabel->setEnabled(false);
  h->addWidget(this->distClusterLabel);
  this->distClusterSlider = new QSlider(Qt::Horizontal);
  this->distClusterSlider->setMaximum(100);
  this->distClusterSlider->setMinimum(1);
  this->distClusterSlider->setValue(1);
  this->distClusterSlider->setEnabled(false);
  h->addWidget(this->distClusterSlider);
  this->distClusterUnits = new QComboBox();
  this->distClusterUnits->addItem("miles");
  this->distClusterUnits->addItem("km");
  this->distClusterUnits->addItem("feet");
  this->distClusterUnits->addItem("m");
  this->distClusterUnits->setCurrentIndex(0);
  this->distClusterUnits->setEnabled(false);
  h->addWidget(this->distClusterUnits);


  /*QtThumbWheel *wheel = new QtThumbWheel();
  wheel->setRange(-100, 100);
  h->addWidget(wheel);
  /*
  /* Add Distamce Cluster to Sep Layout */
  sepLayout->addLayout(h);


  /* */
  sepGroupBox->setLayout(sepLayout);
  vLayout->addWidget(sepGroupBox);
  /* Sets Prefs Widget Layout */
  QFrame *f = new QFrame();
  f->setLayout(vLayout);
  setWidget(f);
  /* Ok nowconnect signals */

  connect(outPathButton,SIGNAL(clicked()),this,SLOT(selectOutDir()));
  connect(inPathButton,SIGNAL(clicked()),this,SLOT(addInDir()));
  connect(this->useFirstTimeCheckBox,SIGNAL(stateChanged(int)),this,SLOT(checkUseFirstTime(int)));
  connect(this->useLastTimeCheckBox,SIGNAL(stateChanged(int)),this,SLOT(checkUseLastTime(int)));
  connect(this->useTimeInterval,SIGNAL(stateChanged(int)),this,SLOT(useTimeIntervalChecked(int)));
  connect(this->useDistanceCluster,SIGNAL(stateChanged(int)),this,SLOT(useDistClusterChecked(int)));
  connect(this->timeIntervalSlider,SIGNAL(valueChanged(int)),this,SLOT(timeIntervalValueChanged(int)));
  connect(this->distClusterSlider,SIGNAL(valueChanged(int)),this,SLOT(distClusterValueChanged(int)));
  connect(this->scanSubs,SIGNAL(stateChanged(int)),this,SLOT(checkedSubs(int)));
}

void QPrefs::checkedSubs(int state) {
  this->needScan = true;
  printf("need scan!\n");
}

void QPrefs::timeIntervalValueChanged(int v) {
  char value[4];
  sprintf(value,"%d",v);
  this->timeIntervalLabel->setText(value);
}
void QPrefs::distClusterValueChanged(int v) {
  char value[4];
  sprintf(value,"%d",v);
  this->distClusterLabel->setText(value);
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
  if (unique) {
    this->inPathList->addItem(inDir);
    this->needScan = true;
  }
}

void QPrefs::checkUseFirstTime(int state) {
  if (state == Qt::Checked ) {
    this->firstDate->setEnabled(true);
    this->firstCalButton->setEnabled(true);
    this->firstTime->setEnabled(true);
    this->fdl->setEnabled(true);
  }
  else {
    this->firstDate->setEnabled(false);
    this->firstCalButton->setEnabled(false);
    this->firstTime->setEnabled(false);
    this->fdl->setEnabled(false);
  }    
}
void QPrefs::checkUseLastTime(int state) {
  if (state == Qt::Checked ) {
    this->lastDate->setEnabled(true);
    this->lastCalButton->setEnabled(true);
    this->lastTime->setEnabled(true);
    this->ldl->setEnabled(true);
  }
  else {
    this->lastDate->setEnabled(false);
    this->lastCalButton->setEnabled(false);
    this->lastTime->setEnabled(false);
    this->ldl->setEnabled(false);
  }   
}
void QPrefs::useTimeIntervalChecked(int state) {
  // For now just turn it back on
  this->useTimeInterval->setCheckState(Qt::Checked);
}

void QPrefs::useDistClusterChecked(int state) {
  // For now just turn it back on
  this->useDistanceCluster->setCheckState(Qt::Unchecked);
}


