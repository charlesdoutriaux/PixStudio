#include <QtGui>
#include <Qt>
#include <preferences.h>
//#include <qtthumbwheel.h>

QPrefs::QPrefs(const QString & title, QWidget * parent = 0, Qt::WindowFlags flags = 0) 
  :QDockWidget(title,parent,flags) {
  setupUi();
};

void QPrefs::setupUi(void) {
  this->dirDialog = new QFileDialog();
  this->vLayout = new QVBoxLayout();
  this->prefsLabel = new QLabel("Preferences");
  this->vLayout->addWidget(prefsLabel);

  /* Paths Related Area */
  this->pathsGroupBox = new QGroupBox();
  this->pathsGroupBox->setTitle("Paths");
  /* GroupBox Layout */
  this->pathsLayout = new QVBoxLayout();

  /* Output Path */
  this->l = new QLabel("Out");
  this->h = new QHBoxLayout();
  this->h->addWidget(this->l);
  this->outLineEdit = new QLineEdit();
  this->outLineEdit->setText(tr("/Users/doutriaux1/Desktop"));
  this->h->addWidget(this->outLineEdit);
  this->outPathButton = new QToolButton();
  this->h->addWidget(this->outPathButton);
  this->pathsLayout->addLayout(this->h);

  /* Input Paths */
  this->v1 = new QVBoxLayout();
  this->h1= new QHBoxLayout();
  this->l1 = new QLabel("In");
  this->h1->addWidget(this->l1);
  this->sp = new QSpacerItem(40,10,QSizePolicy::Expanding,QSizePolicy::Minimum);
  this->h1->addItem(this->sp);
  this->scanSubs = new QCheckBox();
  this->h1->addWidget(this->scanSubs);
  this->l2 = new QLabel("scan subdirectories?");
  this->h1->addWidget(this->l2);
  this->inPathButton = new QToolButton();
  this->h1->addWidget(this->inPathButton);
  this->v1->addLayout(this->h1);
  this->inPathList = new QPathListWidget();
  this->inPathList->addItem(tr("/Users/doutriaux1/Desktop"));
  this->v1->addWidget(this->inPathList);
  this->pathsLayout->addLayout(this->v1);

  /* Setslayout for Paths GroupBox */
  this->pathsGroupBox->setLayout(this->pathsLayout);

  /*Add GroupBox for Paths to Prefs Widget Layout */
  this->vLayout->addWidget(this->pathsGroupBox);

  /* Picture Separation Settings */
  this->sepGroupBox = new QGroupBox();
  this->sepGroupBox->setTitle("Pictures Grouping Selection Criteria");
  /* Separation Layout */
  this->sepLayout = new QVBoxLayout();
  /* First Time */
  this->h2 = new QHBoxLayout();
  this->useFirstTimeCheckBox = new QCheckBox();
  this->h2->addWidget(this->useFirstTimeCheckBox);
  this->fdl = new QLabel("First Time");
  this->fdl->setEnabled(false);
  this->h2->addWidget(this->fdl);
  this->sp1 = new QSpacerItem(40,10,QSizePolicy::Expanding,QSizePolicy::Minimum);
  this->h2->addItem(this->sp1);
  this->firstDate = new QDateEdit();
  this->firstDate->setEnabled(false);
  this->h2->addWidget(this->firstDate);
  this->firstCalButton = new QToolButton();
  this->firstCalButton->setEnabled(false);
  this->h2->addWidget(this->firstCalButton);
  this->firstTime = new QTimeEdit();
  this->firstTime->setEnabled(false);
  this->h2->addWidget(this->firstTime);
  /* Add First Date to Sep Layout */
  //this->sepLayout->addLayout(this->h2);
  /* Second Time */
  this->h3 = new QHBoxLayout();
  this->useLastTimeCheckBox = new QCheckBox();
  //this->h3->addWidget(this->useLastTimeCheckBox);
  this->ldl = new QLabel("Last Time");
  this->ldl->setEnabled(false);
  //this->h3->addWidget(this->ldl);
  this->sp2 = new QSpacerItem(40,10,QSizePolicy::Expanding,QSizePolicy::Minimum);
  this->h3->addItem(this->sp2);
  this->lastDate = new QDateEdit();
  this->lastDate->setEnabled(false);
  this->h3->addWidget(this->lastDate);
  this->lastCalButton = new QToolButton();
  this->lastCalButton->setEnabled(false);
  this->h3->addWidget(this->lastCalButton);
  this->lastTime = new QTimeEdit();
  this->lastTime->setEnabled(false);
  this->h3->addWidget(this->lastTime);
  /* Add Last Date to Sep Layout */
  this->sepLayout->addLayout(this->h3);

  /* Time Interval */
  this->h4 = new QHBoxLayout();
  this->useTimeInterval = new QCheckBox();
  this->useTimeInterval->setCheckState(Qt::Checked);
  this->useTimeInterval->setEnabled(false); // for now
  this->h4->addWidget(this->useTimeInterval);
  this->l3 = new QLabel("Time Interval");
  this->h4->addWidget(this->l3);
  this->timeIntervalLabel = new QLabel("30");
  this->h4->addWidget(this->timeIntervalLabel);
  this->timeIntervalSlider = new QSlider(Qt::Horizontal);
  this->timeIntervalSlider->setMaximum(100);
  this->timeIntervalSlider->setMinimum(1);
  this->timeIntervalSlider->setValue(30);
  this->h4->addWidget(this->timeIntervalSlider);
  this->timeIntervalUnits = new QComboBox();
  this->timeIntervalUnits->addItem("Days");
  this->timeIntervalUnits->addItem("Hours");
  this->timeIntervalUnits->addItem("Minutes");
  this->timeIntervalUnits->addItem("Seconds");
  this->timeIntervalUnits->setCurrentIndex(2);
  this->h4->addWidget(timeIntervalUnits);
  /* Add Time Interval to Sep Layout */
  //this->sepLayout->addLayout(this->h4);

  /* Distance */
  this->h5 = new QHBoxLayout();
  this->useDistanceCluster = new QCheckBox();
  this->useDistanceCluster->setCheckState(Qt::Unchecked);
  this->useDistanceCluster->setEnabled(false);
  this->h5->addWidget(this->useDistanceCluster);
  this->l4 = new QLabel("Distance Cluster");
  this->l4->setEnabled(false);
  this->h5->addWidget(l4);
  this->distClusterLabel = new QLabel("1");
  this->distClusterLabel->setEnabled(false);
  this->h5->addWidget(this->distClusterLabel);
  this->distClusterSlider = new QSlider(Qt::Horizontal);
  this->distClusterSlider->setMaximum(100);
  this->distClusterSlider->setMinimum(1);
  this->distClusterSlider->setValue(1);
  this->distClusterSlider->setEnabled(false);
  this->h5->addWidget(this->distClusterSlider);
  this->distClusterUnits = new QComboBox();
  this->distClusterUnits->addItem("miles");
  this->distClusterUnits->addItem("km");
  this->distClusterUnits->addItem("feet");
  this->distClusterUnits->addItem("m");
  this->distClusterUnits->setCurrentIndex(0);
  this->distClusterUnits->setEnabled(false);
  this->h5->addWidget(this->distClusterUnits);


  /*QtThumbWheel *wheel = new QtThumbWheel();
  wheel->setRange(-100, 100);
  h->addWidget(wheel);*/

  /* Add Distamce Cluster to Sep Layout */
  //this->sepLayout->addLayout(this->h5);


  
  this->sepGroupBox->setLayout(this->sepLayout);
  this->vLayout->addWidget(this->sepGroupBox);
  /* Sets Prefs Widget Layout */
  this->f = new QFrame();
  this->f->setLayout(this->vLayout);
  setWidget(this->f);
  /* Ok nowconnect signals */

  connect(this->outPathButton,SIGNAL(clicked()),this,SLOT(selectOutDir()));
  connect(this->inPathButton,SIGNAL(clicked()),this,SLOT(addInDir()));
  connect(this->useFirstTimeCheckBox,SIGNAL(stateChanged(int)),this,SLOT(checkUseFirstTime(int)));
  connect(this->useLastTimeCheckBox,SIGNAL(stateChanged(int)),this,SLOT(checkUseLastTime(int)));
  connect(this->useTimeInterval,SIGNAL(stateChanged(int)),this,SLOT(useTimeIntervalChecked(int)));
  connect(this->useDistanceCluster,SIGNAL(stateChanged(int)),this,SLOT(useDistClusterChecked(int)));
  connect(this->timeIntervalSlider,SIGNAL(valueChanged(int)),this,SLOT(timeIntervalValueChanged(int)));
  connect(this->distClusterSlider,SIGNAL(valueChanged(int)),this,SLOT(distClusterValueChanged(int)));
  connect(this->scanSubs,SIGNAL(stateChanged(int)),this,SLOT(checkedSubs(int)));
}

void QPrefs::checkedSubs(int state) {
  int i;
  i=state ; // to remove warning at compilation time
  this->needScan = true;
  //printf("need scan!\n");
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
  QString outDir = this->dirDialog->getExistingDirectory(this,"Choose Output Root Directory");
  this->outLineEdit->setText(outDir);
}
void QPrefs::addInDir() {
  int i;
  bool unique;
  QListWidgetItem *item;
  QString inDir = this->dirDialog->getExistingDirectory(this,"Choose Output Root Directory");
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
  int i;
  i=state;
  this->useTimeInterval->setCheckState(Qt::Checked);
}

void QPrefs::useDistClusterChecked(int state) {
  // For now just turn it back on
  int i;
  i=state;
  this->useDistanceCluster->setCheckState(Qt::Unchecked);
}


