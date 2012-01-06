#include <QGalleryTab.h>
#include <mainwindow.h>

QGalleryTab::QGalleryTab(QWidget * parent, struct pix_entries *pix) 
  :QWidget(parent) {

  this->prefs = ((MainWindow *)parent)->prefs;
  this->tabs = ((MainWindow *)parent)->tabs;
  this->iconsSizeSlider = new QSlider(Qt::Horizontal);
  this->iconsSizeSlider->setMaximum(200);
  this->iconsSizeSlider->setMinimum(10);
  this->iconsSizeSlider->setValue(100);


  this->name = new QLineEdit();
  name->setText("");
  //QFrame *f = new QFrame();
  QVBoxLayout *v = new QVBoxLayout();

  v->addWidget(name);
  QHBoxLayout *h = new QHBoxLayout();
  this->iconsSizeLabel = new QLabel("Icons Size: 100px");
  h->addWidget(this->iconsSizeLabel);
  h->addWidget(this->iconsSizeSlider);
  QPushButton *b = new QPushButton();
  b->setIcon(QIcon(":Icons/create_projects.png"));
  QAction *renameAction = new QAction(QIcon(":Icons/create_projects.png"),tr("Rename"),this);
  renameAction->setToolTip(tr("Rename Pix Into Projects Dirs"));
  renameAction->setEnabled(true);
  //b->addAction(renameAction);
  int s = 40;
  b->setIconSize(QSize(s,s));
  //b->setFixedSize(100,100);
  h->addWidget(b);
  v->addLayout(h);
  this->gallery = new QGallery(this,pix);
  v->addWidget(this->gallery);
  setLayout(v);
  connect(this->iconsSizeSlider,SIGNAL(valueChanged(int )),this,SLOT(newIconSize(int)));
  connect(b,SIGNAL(clicked(bool)),this,SLOT(renamePix(bool)));
  // setCentralWidget(f);
  
}

void QGalleryTab::renamePix(bool b) {
  QDir d;
  QMessageBox mbox;
  if (this->prefs->outLineEdit->text().size()==0) {
    mbox.setText("You need to srt an output directory");
    mbox.exec();
    return;
  }
  pix_entries *iter;
  iter= this->gallery->pix;
  time_t t = iter->entry.time;
  char date[25],num[4];
  strftime(date,25,"%Y-%m-%d, ",localtime(&t));
  QString pname = QString(this->prefs->outLineEdit->text()).append(QDir::separator()).append(date).append(this->name->text());
  if (this->name->text().size()==0) {
    mbox.setText("You need to set the project name!");
    mbox.exec();
    return;
  }
  d = QDir(pname);
  if (d.exists()) {
    QStringList files = d.entryList(QDir::Files);
    if (files.count()!=0) {
      mbox.setText(QString("Sorry Project Directory already exists and is not empty: ").append(d.path()));
      mbox.exec();
      return;
    }
  }
  else {
    d.mkpath(d.path());
  }
  int i=0;
  QStringList newfiles;
  while (iter!=NULL) {
    sprintf(num," %.3i.",i);
    i+=1;
    QString in = iter->entry.name;
    QStringList sp = in.split(".");
    QString out = QString(pname).append(QDir::separator()).append(this->name->text()).append(num).append(sp.at(sp.size()-1));
    newfiles.append(out);
    //fprintf(stderr,"would rename: %s to %s\n",iter->entry.name,out.toAscii().data());
    iter=iter->next;
    d.rename(in,out);
  }
  mbox.setText(QString("Renamed files to:\n").append(newfiles.join("\n")));
  mbox.exec();
  this->tabs->removeTab(this->tabs->currentIndex());
  //fprintf(stderr,"out: %s\n",d.path().toAscii().data());

};
void QGalleryTab::newIconSize(int value) {
  char label[50];
  sprintf(label,"Icons Size: %ipx",value);
  //fprintf(stderr,"Slider value changed to: %i\n",value);
  this->iconsSizeLabel->setText(tr(label));
  this->gallery->redraw();
}
