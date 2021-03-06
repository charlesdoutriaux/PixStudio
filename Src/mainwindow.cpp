#include <Qt>
#include <QtGui>
#include <mainwindow.h>
#include <preferences.h>
#include <files_parsing.h>
#include <QGalleryTab.h>

MainWindow::MainWindow() 
  :QMainWindow() {
  setupUi();
  connectSignals();
}

void MainWindow::setupUi() {
  this->prefs = new QPrefs(tr("Preferences Dock"),this,0);
  addDockWidget(Qt::LeftDockWidgetArea,this->prefs);
  //centralWidget->addWidget(prefs);
  //printf("Ok dist label is: %s\n",(const char *) prefs->distClusterLabel->text().toAscii());
  this->tabs = new QTabWidget();
  this->tabs->setTabsClosable(true);
  this->tabs->setMovable(true);
  this->tabs->setUsesScrollButtons(true);
  this->setCentralWidget(this->tabs);
  setWindowTitle("PixStudio");
  setWindowIcon(QIcon(":Icons/app.png"));
  this->tb = new QToolBar();
  this->tb->setFloatable(true);
  addToolBar(this->tb);
  QMenuBar *menu = new QMenuBar();
  menu->addMenu(tr("&View"));
  this->pix=NULL;
}

void MainWindow::connectSignals() {
  QAction *refreshAction = new QAction(QIcon(":Icons/refresh.png"),tr("Refresh"),this);
  refreshAction->setToolTip(tr("Refresh Projects"));
  refreshAction->setEnabled(true);
  connect(refreshAction,SIGNAL(triggered()),this,SLOT(refreshProjects()));
  this->tb->addAction(refreshAction);
  QAction *renameAction = new QAction(QIcon(":Icons/create_projects.png"),tr("Rename"),this);
  renameAction->setToolTip(tr("Rename Pix Into Projects Dirs"));
  renameAction->setEnabled(true);
  connect(renameAction,SIGNAL(triggered()),this,SLOT(renamePix()));
  connect(this->tabs,SIGNAL(currentChanged(int)),this,SLOT(newTabSelected(int)));
  connect(tabs,SIGNAL(tabCloseRequested(int)),this,SLOT(closeATab(int)));
  this->tb->addAction(renameAction);

}

void MainWindow::closeATab(int i) {
  tabs->removeTab(i);
}
void MainWindow::newTabSelected(int tab) {
  if (tabs->count()==0) return;
  QGalleryTab *aTab = (QGalleryTab *)(this->tabs->widget(tab));
  aTab->gallery->redraw();
};

void MainWindow::renamePix() {
  fprintf(stderr,"Ok would rename the files here\n");
}

void MainWindow::refreshProjects() {
  struct pix_entries *iter,**split;
  char path[NAME_MAX_LENGTH];
  int i,seconds;

  QString outPath = this->prefs->outLineEdit->text();
  if (outPath.size()==0) {
    QMessageBox mBox;
    mBox.setText("You need to set an output path");
    mBox.exec();
    return;
  }
  if (this->prefs->inPathList->count()==0) {
    QMessageBox mBox;
    mBox.setText("You need to choose some directory to scan for pictures.");
    mBox.exec();
    return;
  }
  while (this->tabs->count()>0) {
    this->tabs->removeTab(0);
  }
  //fprintf(stderr,"needScan: %i\n",this->prefs->needScan);
  if (this->prefs->needScan) {
    //printf("Here we scan the directories\n");
    if (this->pix!=NULL) {
      //printf("Cleaning old one!\n");
      entriesFree(this->pix);
    }
    this->pix = (struct pix_entries *)malloc(sizeof(struct pix_entry));
    entriesInit(this->pix);
    //entriesPrint(this->pix);
    // Ok now scan all dirs
    //fprintf(stderr,"count: %i\n",this->prefs->inPathList->count());
    for(i=0;i<this->prefs->inPathList->count();i++) {
      strcpy(path,this->prefs->inPathList->item(i)->text().toAscii().data());
      //fprintf(stderr,"PATH: %s\n",path);
      if (this->prefs->scanSubs->checkState()==Qt::Checked) {
	scanDir(path,this->pix,-1);
      }
      else {
	scanDir(path,this->pix,0);
      }
    }
    /* Ok seems like we need to sort this thing! */
    entriesQuicksort(this->pix,0,entriesLen(this->pix)-1);

  }
  if (this->pix == NULL) {
     QMessageBox mBox;
    mBox.setText("No Files Found!");
    mBox.exec();
    return;
  };
  // Ok here insert code for cropping
  //crop(this->pix);
  //Now we need to break it by time
  seconds = this->prefs->timeIntervalSlider->value();
  if (this->prefs->timeIntervalUnits->currentText().compare(QString("Days"))==0) {
    seconds*=86400;
  }
  else if (this->prefs->timeIntervalUnits->currentText().compare(QString("Hours"))==0) {
    seconds*=3600;
  }
  else if (this->prefs->timeIntervalUnits->currentText().compare(QString("Minutes"))==0) {
    seconds*=60;
  }
  split = entriesSplitPerTime(this->pix,seconds);
  i=0;
  iter=split[0];
  while (iter!=NULL) {
    time_t t = iter->entry.time;
    char date[11];
    strftime(date,11,"%Y-%m-%d",localtime(&t));
    sprintf(path,"%s (%i)",date,i);
    this->tabs->addTab(new QGalleryTab(this,iter),tr(path));
    //printf("Pack: %i, n%s\n",i,iter->entry.name);
    //entriesFree(iter);
    //printf("len: %i\n",entriesLen(&split[i]));
    i+=1;
    iter=split[i];
  }    
}
