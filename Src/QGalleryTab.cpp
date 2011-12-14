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
  v->addLayout(h);
  this->gallery = new QGallery(this,pix);
  v->addWidget(this->gallery);
  setLayout(v);
  // setCentralWidget(f);
  
}
