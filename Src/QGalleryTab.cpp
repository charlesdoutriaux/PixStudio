#include <QGalleryTab.h>

QGalleryTab::QGalleryTab(QWidget * parent, struct pix_entries *pix) 
  :QWidget(parent) {

  this->iconsSizeSlider = new QSlider(Qt::Horizontal);
  this->iconsSizeSlider->setMaximum(200);
  this->iconsSizeSlider->setMinimum(10);
  this->iconsSizeSlider->setValue(100);

  //QFrame *f = new QFrame();
  QVBoxLayout *v = new QVBoxLayout();
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
  fprintf(stderr,"Ok renaming pix\n");
};
void QGalleryTab::newIconSize(int value) {
  char label[50];
  sprintf(label,"Icons Size: %ipx",value);
  //fprintf(stderr,"Slider value changed to: %i\n",value);
  this->iconsSizeLabel->setText(tr(label));
  this->gallery->redraw();
}
